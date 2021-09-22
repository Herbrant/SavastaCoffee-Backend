FROM gcc:latest
RUN apt-get update && apt-get install -y libulfius-dev uwsc sqlite3 \
	&& apt-get clean \
 	&& rm -rf /var/lib/apt/lists/*

WORKDIR /app/

COPY * ./
RUN make

ENTRYPOINT [ "/app/app" ]