# v1.0.0 Final Migration prior to Version 1.0.0

(August 20, 2023)

### Remove old style caches

Version 1.0.0 is here! As part of this accomplishment, we've removed a lot of old code related to caching. We tried very hard to do this in a backward compatible way, but, alas, we could not. For this reason, in order to use release v1.0.0 and later, you must remove the old cache.

You will continue to receive the message that brought you to this migration until you complete it.

The migration is simple. Do this:

```
chifra config --paths | grep Cache
```

This will report the location of your existing cache. You must move (or remove) this folder to a different location. Don't worry, it will be re-created automatically. For example, if the above command reports that your cache is in `/home/you/.chifra/Cache`, then do this:

```
mv /home/you/.chifra/Cache /home/you/savedCache
```

Note, that unless you intend to return to a previous version, you do not need to preserve the old cache. You could simply remove it.

## Yeah, but won't that mean I have to re-generate my cache?

Unfortunately, yes. There's nothing we can do. If it's any consolation, part of moving to version v1.0.0 means that we will never again create a backwards incompatible change without an automated migration path. The amount of crud that had accumulated over the five year development cycle made it impossible to do that this time. Sorry.

Also, another consolation is that the cache is much faster now. It's also smaller and more capable (caching many more commands). So, it will take less time to re-generate than originally and take up less space to store.

## You're finished

You'll know you're finished if you can run commands without getting the message. Try `chifra blocks 12`.

## Previous Migration

[Click here](./README-v0.70.0.md) for the previous migration.
