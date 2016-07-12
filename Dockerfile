FROM quay.io/aptible/alpine:3.4

ADD . /app
WORKDIR /app

# Add Tini so that the hog can receive SIGTERM
RUN apk-install tini

# Add the hog itself
RUN apk-install build-base \
 && gcc hog.c -o /usr/local/bin/hog \
 && apk del build-base

ENTRYPOINT ["/sbin/tini", "--"]
CMD ["hog"]
