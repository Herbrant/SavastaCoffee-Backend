FROM gcc:latest
RUN apt-get update && apt install -y libulfius-dev uwsc sqlite3
WORKDIR /app/

COPY * ./
RUN make

ENTRYPOINT [ "/app/app" ]