# Exact uint64 OIDs in market proof artifacts

Core3 object IDs are unsigned 64-bit values. JSON itself can represent these
integers, and nlohmann/json preserves them inside Core3, but shell-side `jq`
uses an IEEE-754 numeric representation on common builds. Converting a large
OID from a JSON number with `tostring` can therefore silently round it.

FURY probe and fixture manifests now emit decimal-string companions for every
64-bit identity used by the Docker harness. Persistent object graphs also emit
`persistentObjectIdsText`.

The verifier compares OID string fields and indexes
`persistentObjectPresence` only with those exact strings. Numeric fields are
retained for Core3-native consumers and backward compatibility, but are not
authoritative in shell orchestration.

Docker verifier smoke coverage uses values near the uint64 limit to ensure the
proof path never depends on floating-point OID conversion.
