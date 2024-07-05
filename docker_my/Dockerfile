# Use an official Ubuntu as a parent image
FROM ubuntu:22.04

# Set the working directory
WORKDIR /data

# Set environment variable to avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive
ENV DB_URI=mongodb://localhost/open5gs

# Install dependencies
RUN apt-get update && \
    apt-get install -y \
    gnupg \
    curl \
    python3-pip \
    python3-setuptools \
    python3-wheel \
    ninja-build \
    build-essential \
    flex \
    bison \
    git \
    cmake \
    libsctp-dev \
    libgnutls28-dev \
    libgcrypt-dev \
    libssl-dev \
    libidn11-dev \
    libmongoc-dev \
    libbson-dev \
    libyaml-dev \
    libnghttp2-dev \
    libmicrohttpd-dev \
    libcurl4-gnutls-dev \
    libtins-dev \
    libtalloc-dev \
    meson \
    iproute2 \
    tzdata && \
    echo "America/Detroit" > /etc/timezone && \
    ln -fs /usr/share/zoneinfo/America/Detroit /etc/localtime && \
    dpkg-reconfigure -f noninteractive tzdata

# Clone the repository
RUN git clone https://github.com/JhengLu/open5gs_my.git /data/open5gs_my

# Add MongoDB key and repository
RUN curl -fsSL https://pgp.mongodb.com/server-6.0.asc | gpg -o /usr/share/keyrings/mongodb-server-6.0.gpg --dearmor && \
    echo "deb [ arch=amd64,arm64 signed-by=/usr/share/keyrings/mongodb-server-6.0.gpg] https://repo.mongodb.org/apt/ubuntu jammy/mongodb-org/6.0 multiverse" | tee /etc/apt/sources.list.d/mongodb-org-6.0.list

# Install MongoDB
RUN apt-get update && \
    apt-get install -y mongodb-org

# Build Open5GS
WORKDIR /data/open5gs_my
RUN meson build --prefix=`pwd`/install && \
    ninja -C build

# Install Open5GS
WORKDIR /data/open5gs_my/build
RUN ninja install

# Create the data directory
RUN mkdir -p /data/db

# Expose the necessary ports (if any)
EXPOSE 3000 27017 80 443

# Start MongoDB and set up TUN device in CMD
CMD ["bash", "-c", "\
    mongod --bind_ip_all --logpath /data/mongodb.log --dbpath /data/db & \
    sleep 5 && \
    ip tuntap add name ogstun mode tun && \
    ip addr add 10.45.0.1/16 dev ogstun && \
    ip addr add 2001:db8:cafe::1/48 dev ogstun && \
    ip link set ogstun up && \
    exec tail -f /dev/null"]
