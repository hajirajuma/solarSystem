#!/usr/bin/env bash

# Replace these values
OLD_NAME="victormuntanya"
OLD_EMAIL=""

NEW_NAME="hajirajuma"
NEW_EMAIL="hajiraj475@gmail.com"

git filter-branch --env-filter '

if [ "$GIT_AUTHOR_NAME" = "'"$OLD_NAME"'" ] || [ "$GIT_AUTHOR_EMAIL" = "'"$OLD_EMAIL"'" ]
then
    export GIT_AUTHOR_NAME="'"$NEW_NAME"'"
    export GIT_AUTHOR_EMAIL="'"$NEW_EMAIL"'"
fi

if [ "$GIT_COMMITTER_NAME" = "'"$OLD_NAME"'" ] || [ "$GIT_COMMITTER_EMAIL" = "'"$OLD_EMAIL"'" ]
then
    export GIT_COMMITTER_NAME="'"$NEW_NAME"'"
    export GIT_COMMITTER_EMAIL="'"$NEW_EMAIL"'"
fi

' --tag-name-filter cat -- --branches --tags

echo "Done rewriting commit history."
echo "Now run:"
echo "git push --force --all"
echo "git push --force --tags"