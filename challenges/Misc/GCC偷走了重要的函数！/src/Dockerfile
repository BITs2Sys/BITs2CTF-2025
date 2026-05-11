FROM ubuntu:24.04

RUN apt-get update && apt-get install -y gcc adduser python3 python3-pip wget socat

RUN adduser --disabled-password --home /home/ctf ctf

RUN su ctf -c 'wget -qO- https://astral.sh/uv/install.sh | sh'
ENV PATH="/home/ctf/.local/bin/:$PATH"

RUN mkdir -p /home/ctf/build
COPY ./src /home/ctf/src

RUN chown -R ctf:ctf /home/ctf/src/
RUN chown -R ctf:ctf /home/ctf/build/
RUN cd /home/ctf/src && su ctf -c '/home/ctf/.local/bin/uv sync'

COPY ./run.sh /

EXPOSE 10001
CMD ["socat", "tcp-listen:10001,fork", "exec:/run.sh,reuseaddr"]

