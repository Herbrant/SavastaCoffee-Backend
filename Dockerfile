FROM gcc:latest
RUN apt update && apt install -y libulfius-dev uwsc sqlite3
WORKDIR /app/

ADD * ./
RUN make

ENTRYPOINT [ "/app/app" ]