# TRE volume bootstrap

The Docker runtime uses `TRE_VOLUME` (default: `shared-tre`) and mounts it
read-only at `/tre`.

After building the image:

```bash
cd docker
./tre.sh import /path/to/SWG/files
./tre.sh status
```

Use `--replace` for a clean re-import.
