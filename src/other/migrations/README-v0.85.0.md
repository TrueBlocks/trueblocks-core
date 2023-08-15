# v0.85.0 Final Migration but One for Version v1.0.0

(August 20, 2023)

### Remove old style caches

We are quickly approaching version 1.0.0 and as part of that we've removed a lot of old code related to our caching. We tried very hard to do this in a backward compatible way, we it became impossible after a while. For this reason, in order to use release v0.85.0 and later, you must remove the old cache.

You will continue to get the message that sent you here until you do.

The migration is very simple.

```
chifra config --path | grep Cache
```

Move or remove the resulting folder from your computer. It will be re-created.

## Yeah, but won't that mean I have to re-generate my cache?

Unfortunately, yes. There's nothing we can do. If it's any consolation, part of moving to version v1.0.0 means that we will never again create a backwards incompatible change without an automated migration path. The amount of crud that had accumulated over the five year development cycle made it impossible to do that this time. Sorry.

## You're finished

You're finished.

## Previous Migration

[Click here](./README-v0.70.0.md) for the previous migration.
