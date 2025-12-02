#!/bin/bash
set -e
echo "=== Configuring primary ==="

# Append configuration to postgresql.conf as postgres user
docker exec -u postgres pg-primary bash -c "
echo \"wal_level = replica\" >> /var/lib/postgresql/data/postgresql.conf
echo \"max_wal_senders = 10\" >> /var/lib/postgresql/data/postgresql.conf
echo \"max_replication_slots = 10\" >> /var/lib/postgresql/data/postgresql.conf
echo \"synchronous_standby_names = 'ANY 2 (sync1, sync2)'\" >> /var/lib/postgresql/data/postgresql.conf
"

# Allow replication connections
docker exec -u postgres pg-primary bash -c "
echo \"host replication repl 0.0.0.0/0 trust\" >> /var/lib/postgresql/data/pg_hba.conf
echo \"host all all 0.0.0.0/0 trust\" >> /var/lib/postgresql/data/pg_hba.conf
"

# Create replication role
docker exec pg-primary psql -U postgres -c \
  "CREATE ROLE repl WITH REPLICATION LOGIN PASSWORD 'repl';"

# Reload config
docker exec -u postgres pg-primary bash -c "pg_ctl -D /var/lib/postgresql/data reload"
