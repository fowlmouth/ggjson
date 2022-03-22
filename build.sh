#!/bin/sh

## run a multi platform build with docker

set -e

pcmd(){
  echo "$@"
  "$@"
}

[ -z "$REPO" ] && REPO="fowlmouth/ggjson"
[ -z "$TAG" ] && TAG="$(git rev-parse HEAD)"
[ -z "$PLATFORMS" ] && PLATFORMS="arm64 amd64"

pcmd make clean

for PLATFORM in $PLATFORMS; do
  THIS_IMAGE="$REPO:$TAG-$PLATFORM"
  pcmd docker build . --platform "linux/$PLATFORM" --tag "$THIS_IMAGE"
  pcmd docker run "$THIS_IMAGE" ./tests
  pcmd docker push "$REPO"
  MANIFEST_IMAGES+="$THIS_IMAGE "
done

pcmd docker manifest create "$REPO:$TAG" $MANIFEST_IMAGES
pcmd docker manifest push "$REPO:$TAG"