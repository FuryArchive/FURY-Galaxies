# Docker runtime harness for FURY

The inherited Core3 Docker developer image now defaults to the FURY fork:

- organization/base URL: `https://github.com/FuryArchive`
- repository: `FURY-Galaxies`
- branch: `fury-dev`

The checkout destination inside the container intentionally remains
`~/workspace/Core3`. Existing SQL/config/build paths depend on that workspace
name and do not need to know the public repository name.

A new `REPO_PUBLIC_NAME` environment value controls the repository component
of the clone URL. This keeps the Docker scaffold reusable for upstream Core3 by
overriding:

```
REPO_PUBLIC_URL=https://github.com/swgemu
REPO_PUBLIC_NAME=Core3
REPO_PUBLIC_BRANCH=unstable
```

This removes the first blocker for the FURY crash-recovery runtime canary.
The remaining external prerequisite is the normal SWG TRE data mounted at
`/tre`; those proprietary game assets cannot be supplied by public CI.
