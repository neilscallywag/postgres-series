#!/bin/bash
set -e

echo "=== Creating basebackups for standbys ==="

# Path to Postgres 17 binaries
PG_BIN="/usr/lib/postgresql/17/bin"

# Standbys and their application_names
STANDBYS=("sync1" "sync2" "async1")

for node in "${STANDBYS[@]}"; do
  echo "--- Initializing $node ---"

  # Stop any postgres processes
  docker exec pg-$node bash -c "pkill -u postgres || true"

  # Wipe the data directory
  docker exec pg-$node bash -c "rm -rf /var/lib/postgresql/data/*"

  # Run pg_basebackup to clone from primary
  docker exec pg-$node bash -c "
    pg_basebackup -h primary -U repl -D /var/lib/postgresql/data -Fp -Xs -P -R
  "

  # Fix ownership + permissions
  docker exec pg-$node bash -c "chown -R postgres:postgres /var/lib/postgresql/data"
  docker exec pg-$node bash -c "chmod -R 700 /var/lib/postgresql/data"

  # Overwrite the broken auto-generated primary_conninfo
  # Set correct password and application_name
  docker exec pg-$node bash -c "
    printf \"primary_conninfo = 'user=repl password=repl host=primary port=5432 application_name=$node'\" \
    > /var/lib/postgresql/data/postgresql.auto.conf
  "

done


echo "=== Starting Postgres on standbys ==="

for node in "${STANDBYS[@]}"; do
  docker exec pg-$node bash -c \
    "su - postgres -c 'PATH=$PG_BIN:\$PATH postgres -D /var/lib/postgresql/data >> /var/lib/postgresql/data/log 2>&1 &'"
done

echo "=== Standbys started ==="
