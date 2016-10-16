Synchronization
===============

Methos
------

| # | Method | uses FiFo | Details | 
|---|--------|-----------|----------|
| 1 | raw observation value | false | observed value is taken without smoothing |
| 2 | progressive mean | false | average of last and new value is taken (simple smoothing) |
| 3 | mean | true | mean of values stored in FiFo |
| 4 | mean without outlier | true | values withing fifo's µ +/- n*σ are considered for the mean |
| 5 | mean without marked outlier | true | values entering FiFo are marked as outlier/valid according to current FiFo's N(µ,σ)|
| 6 | mean with adaptive outlier detection | true | counted outlier vs. counted valid must hold a ratio, limit is in-/decreased accordingly: µ+/-limit|
| 7 | lin. regression (least square) | true | https://en.wikipedia.org/wiki/Linear_least_squares_(mathematics) |


Test Setup
----------
| # | Method# | Method | FiFo size | outlier detection σ  | pitch | performance |  
|---|------------|-----|-----------|----------------------|-------|-------------|
| 1 | 1          | raw value |     |                      | 12    | good        |
| 2 | 3          | mean | 40 |     |                      | 16    | slow adjustment |
| 3 | 3          | mean | 8  |     |                      | 16    | good |
| 4 | 5          | mean without marked outlier | 80 | 2.0 | 16   | slow |
| 5 | 4          | mean without outlier | 16 | 2.0 | 16   | acceptable |
| 6 | 4          | mean without outlier | 40 | 2.0 | 20   | slow |
| 7 | 4          | mean without outlier | 40 | 2.0 | 16   | good |
| 8 | 2          | progressive mean     |    |     | 16   | good |
| 9 | 7          | linear regression    | 40 |     |      | poor, failed, overshoot |
|   | 6          | TODO                 |    |     |      |                    | 
