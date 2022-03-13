#!/bin/sh

## run a multi platform build with docker

pcmd(){
  echo "$@"
  "$@"
}

if [ -z "$IMAGE" ]; then
  [ -z "$TAG" ] && TAG="$(git rev-parse HEAD)"

  IMAGE="fowlmouth/ggjson:$TAG"
fi

if [ -z "$PLATFORMS" ]; then
  PLATFORMS="linux/arm64,linux/amd64"
fi

printf "running build image='%s' platforms='%s'" "$IMAGE" "$PLATFORMS"

pcmd make clean
pcmd make deps

if [ "$(docker buildx ls | grep '*' | cut -d' ' -f1)" = "default" ]; then
  pcmd docker buildx create --use
fi

pcmd docker buildx build . --platform "$PLATFORMS" --tag "$IMAGE" --push

