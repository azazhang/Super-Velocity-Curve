#!/usr/bin/env bash
# Bounded CI watcher — avoids unbounded `gh run watch` background hangs in agent shells.
set -euo pipefail

REPO="${GITHUB_REPOSITORY:-azazhang/Super_VelocityCurve}"
RUN_ID="${1:-$(gh run list --repo "$REPO" --limit 1 --json databaseId -q '.[0].databaseId')}"
MAX_WAIT_SEC="${CI_WATCH_TIMEOUT_SEC:-2700}"  # 45 minutes
POLL_SEC="${CI_WATCH_POLL_SEC:-30}"

if [[ -z "$RUN_ID" || "$RUN_ID" == "null" ]]; then
  echo "No workflow run id found." >&2
  exit 1
fi

echo "Watching run $RUN_ID on $REPO (timeout ${MAX_WAIT_SEC}s, poll every ${POLL_SEC}s)"
deadline=$((SECONDS + MAX_WAIT_SEC))

while (( SECONDS < deadline )); do
  read -r status conclusion <<< "$(gh run view "$RUN_ID" --repo "$REPO" \
    --json status,conclusion -q '"\(.status) \(.conclusion // "none")"')"

  if [[ "$status" == "completed" ]]; then
    echo "Run $RUN_ID finished: $conclusion"
    gh run view "$RUN_ID" --repo "$REPO"
    [[ "$conclusion" == "success" ]]
    exit $?
  fi

  echo "$(date -u +%H:%M:%S) status=$status"
  sleep "$POLL_SEC"
done

echo "Timed out after ${MAX_WAIT_SEC}s waiting for run $RUN_ID" >&2
gh run view "$RUN_ID" --repo "$REPO" || true
exit 124
