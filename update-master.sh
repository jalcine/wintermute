#!/bin/bash
#
# Synchronizes all of the code in the
# branches so that everyone's up-to-date.

BRANCHES="
core
wapi-network
wapi-data
wapi-linguistics
wapi-visual"

function mergeWithBranches {
    for branch in $BRANCHES
    do
        retval=$(git merge $branch);
        echo "**** Merging with branch '$branch' ****"
    done
}

function updateBranches {
    for branch in $BRANCHES
    do
        echo "**** Checking out branch '$branch' ****"
        co=$(git checkout $branch);
        echo "**** Updating branch '$branch' ****"
        mergeWithBranches
    done
}

updateBranches