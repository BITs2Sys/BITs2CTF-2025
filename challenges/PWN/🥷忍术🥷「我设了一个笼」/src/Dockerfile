FROM ubuntu:24.04

RUN apt-get update && apt-get install -y gcc adduser python3 python3-pip wget socat make

RUN adduser --disabled-password --home /home/ctf ctf


COPY ./src /home/ctf/src
COPY ./jail /home/ctf/jail

RUN chown -R ctf:ctf /home/ctf/
RUN mkdir -p /home/ctf/jail/home/hamood/enter/this/dir/you/will/see/flag \
  && echo 'hamood?hamood?hamood!' > /home/ctf/jail/home/hamood/enter/this/dir/you/will/see/flag/IamFlag

RUN cd /home/ctf/src && make

COPY ./flag /flag
COPY ./run.sh /run.sh

RUN chmod +r /flag && chmod +x /run.sh

EXPOSE 10001
CMD ["socat", "tcp-listen:10001,fork", "exec:/run.sh,reuseaddr"]

