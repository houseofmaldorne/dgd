# include "dgd.h"
# include "str.h"
# include "array.h"
# include "object.h"
# include "xfloat.h"
# include "interpret.h"
# include "data.h"
# include "path.h"
# include "node.h"
# include "compile.h"

/*
 * NAME:	path->resolve()
 * DESCRIPTION:	resolve a path
 */
char *path_resolve(buf, file)
char *buf, *file;
{
    register char *p, *q, *d;

    strncpy(buf, file, STRINGSZ - 1);
    buf[STRINGSZ - 1] = '\0';
    d = p = q = buf;
    for (;;) {
	if (*p == '/' || *p == '\0') {
	    /* reached a directory separator */
	    if (q - 1 == d && d[0] == '.') {
		/* . */
		q = d;
	    } else if (q - 2 == d && d[0] == '.' && d[1] == '.') {
		/* .. */
		q = d;
		if (q != buf) {
		    for (--q; q != buf && *--q != '/'; ) ;
		}
	    }
	    if (q != buf) {
		if (q[-1] == '/') {
		    /* // or path/ */
		    --q;
		}
		*q++ = *p;
	    }
	    d = q;
	    if (*p == '\0') {
		break;
	    }
	    p++;
	} else {
	    *q++ = *p++;
	}
    }

    if (q == buf) {
	/* "" -> "." */
	*q++ = '.';
	*q = '\0';
    }
    return buf;
}

/*
 * NAME:	path_string()
 * DESCRIPTION:	check and resolve a string path
 */
char *path_string(buf, file, len)
char *buf, *file;
unsigned int len;
{
    if (len >= STRINGSZ || strlen(file) != len) {
	return (char *) NULL;
    }
    return path_resolve(buf, file);
}

/*
 * NAME:	path->from()
 * DESCRIPTION:	resolve a (possibly relative) path
 */
char *path_from(buf, from, file)
register char *buf, *from, *file;
{
    char buf2[STRINGSZ];

    if (file[0] != '/' && strlen(from) + strlen(file) < STRINGSZ - 4) {
	sprintf(buf2, "%s/../%s", from, file);
	file = buf2;
    }
    return path_resolve(buf, file);
}

/*
 * NAME:	path->ed_read()
 * DESCRIPTION:	resolve an editor read file path
 */
char *path_ed_read(buf, file)
char *buf, *file;
{
    register frame *f;

    f = cframe;
    if (f->obj->flags & O_DRIVER) {
	return path_resolve(buf, file);
    } else {
	(--f->sp)->type = T_STRING;
	str_ref(f->sp->u.string = str_new(file, (long) strlen(file)));
	call_driver_object(f, "path_read", 1);
	if (f->sp->type != T_STRING) {
	    i_del_value(f->sp++);
	    return (char *) NULL;
	}
	path_resolve(buf, f->sp->u.string->text);
	str_del((f->sp++)->u.string);
	return buf;
    }
}

/*
 * NAME:	path->ed_write()
 * DESCRIPTION:	resolve an editor write file path
 */
char *path_ed_write(buf, file)
char *buf, *file;
{
    register frame *f;

    f = cframe;
    if (f->obj->flags & O_DRIVER) {
	return path_resolve(buf, file);
    } else {
	(--f->sp)->type = T_STRING;
	str_ref(f->sp->u.string = str_new(file, (long) strlen(file)));
	call_driver_object(f, "path_write", 1);
	if (f->sp->type != T_STRING) {
	    i_del_value(f->sp++);
	    return (char *) NULL;
	}
	path_resolve(buf, f->sp->u.string->text);
	str_del((f->sp++)->u.string);
	return buf;
    }
}

/*
 * NAME:	path->include()
 * DESCRIPTION:	resolve an include path
 */
char *path_include(buf, from, file)
char *buf, *from, *file;
{
    register frame *f;

    if (c_autodriver()) {
	return path_from(buf, from, file);
    }
    f = cframe;
    (--f->sp)->type = T_STRING;
    str_ref(f->sp->u.string = str_new((char *) NULL, strlen(from) + 1L));
    f->sp->u.string->text[0] = '/';
    strcpy(f->sp->u.string->text + 1, from);
    (--f->sp)->type = T_STRING;
    str_ref(f->sp->u.string = str_new(file, (long) strlen(file)));
    if (!call_driver_object(f, "path_include", 2)) {
	f->sp++;
	return path_from(buf, from, file);
    }
    if (f->sp->type != T_STRING) {
	i_del_value(f->sp++);
	return (char *) NULL;
    }
    path_resolve(buf, f->sp->u.string->text);
    str_del((f->sp++)->u.string);
    return buf;
}
