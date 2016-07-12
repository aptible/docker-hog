A simple configurable hog program to simulate apps that exceed their memory
allocation.

## Configuration ##

+ `HOG_MAX_BYTES`: How much memory to hog before exiting. Default is
  `LONG_MAX`, which essentially means no limit. Set to e.g. `2147483648` to hog
  up to 2 GiB.
+ `HOG_WAIT_SECONDS`: How long to wait before we start hogging. Default is 0,
  which means don't wait at all. Set to e.g. `2` to wait 2 seconds before the
  hogging begins.
+ `HOG_GOAL_SECONDS`: how long should the allocation of `HOG_MAX_BYTES` happen
  over? Default is `0`, which means hog as fast as possible. Set to e.g. `60`
  for the hogging to happen over a minute.


## Copyright and License ##

MIT License, see [LICENSE](LICENSE.md) for details.

Copyright (c) 2016 [Aptible](https://www.aptible.com) and contributors.
