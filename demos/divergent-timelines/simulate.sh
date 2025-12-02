#!/bin/bash
set -e

docker-compose down -v
docker-compose build

echo "=== Starting primary only ==="
docker-compose up -d primary

sleep 6
./init-primary.sh

echo "=== Creating base backups for standbys ==="
docker-compose up -d sync1 sync2 async1
sleep 3
./init-standbys.sh

echo "=== Starting Postgres on standbys ==="


docker exec pg-sync1 bash -c \
  "su - postgres -c 'PATH=/usr/lib/postgresql/17/bin:$PATH postgres -D /var/lib/postgresql/data >> /var/lib/postgresql/data/log 2>&1 &'"

docker exec pg-sync2 bash -c \
  "su - postgres -c 'PATH=/usr/lib/postgresql/17/bin:$PATH postgres -D /var/lib/postgresql/data >> /var/lib/postgresql/data/log 2>&1 &'"

docker exec pg-async1 bash -c \
  "su - postgres -c 'PATH=/usr/lib/postgresql/17/bin:$PATH postgres -D /var/lib/postgresql/data >> /var/lib/postgresql/data/log 2>&1 &'"

sleep 5
