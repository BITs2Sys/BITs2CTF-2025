FROM andyshinn/dnsmasq:latest

COPY dnsmasq.conf /etc/dnsmasq.conf.template
COPY hosts /etc/hosts

RUN echo '#!/bin/sh' > /entrypoint.sh && \
    echo 'sed "s|BITs2CTF{test_flag}|${GZCTF_FLAG}|g" /etc/dnsmasq.conf.template > /etc/dnsmasq.conf' >> /entrypoint.sh && \
    echo 'exec dnsmasq --no-daemon' >> /entrypoint.sh && \
    chmod +x /entrypoint.sh

EXPOSE 53/tcp
EXPOSE 53/udp

ENTRYPOINT ["/entrypoint.sh"]

