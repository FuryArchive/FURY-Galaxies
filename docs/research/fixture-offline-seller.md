# Offline seller support for deterministic fixture

The deterministic market fixture accepts an existing character OID without
requiring the character to be online. If the credit object is not already
loaded, FURY derives its persistent `credits` OID, loads it through
`ObjectManager`, and accepts it only when its owner OID exactly matches the
configured seller.
