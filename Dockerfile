
# #### #### #### #### #### #### #### #### #### #### #### #### ####
#  build using debian bookworm (12)
# #### #### #### #### #### #### #### #### #### #### #### #### ####

FROM --platform=linux/amd64 debian:12-slim

LABEL org.opencontainers.image.source=https://github.com/maldorne/dgd
LABEL org.opencontainers.image.description="DGD mud driver"
LABEL org.opencontainers.image.licenses=AGPL-3.0

# install needed tools
RUN apt-get update && apt-get install -y --force-yes git make gcc clang bison

# create group and user, with uids
RUN groupadd -g 4200 mud
RUN useradd -u 4201 -g 4200 -ms /bin/bash mud
USER mud

WORKDIR /opt/mud
COPY --chown=mud:mud . /opt/mud/driver/

WORKDIR /opt/mud/driver/src
RUN make clean
RUN make
RUN make install

# let's copy the binary to the same directory as other images
RUN cp -R /opt/mud/driver/bin/ /opt/mud/

WORKDIR /opt/mud/

# expose telnet mudos ports
EXPOSE 5000/tcp
