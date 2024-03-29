FROM debian:12
RUN apt-get update && apt-get dist-upgrade -y

# debian 12 has KiCad 6.0 by default - installing 7.0 from newer rc instead
#RUN apt-get update && apt-get install -y --no-install-recommends \
#    kicad \
#    kicad-packages3d
COPY download_kicad /tmp/
RUN apt-get update && apt-get install -y --no-install-recommends \
      wget \
      ca-certificates \
      openssl && \
    cd /tmp/ && \
    ./download_kicad '7.0' && \
    apt-get install -y ./*.deb && \
    rm *.deb

RUN apt-get update && apt-get install -y --no-install-recommends \
    prusa-slicer

RUN apt-get update && apt-get install -y --no-install-recommends \
    fakeroot \
    make \
    gzip \
    tar \
    zip

COPY \
  generate_bom \
  generate_stl \
  list_cu_layers \
  print_on_error \
  /usr/local/bin/
