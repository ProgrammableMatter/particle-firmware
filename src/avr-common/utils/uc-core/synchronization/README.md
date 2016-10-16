Synchronization Methods
=======================


| # | Method | uses FiFo | Details | 
|---|--------|-----------|----------|
| 1 | raw observation value | false | observed value is taken without smoothing |
| 2 | progressive mean | false | average of last and new value is taken (simple smoothing) |
| 3 | mean | true | mean of values stored in FiFo |
| 4 | mean without outlier | true | values withing fifo's µ +/- n*σ are considered for the mean |
| 5 | mean without marked outlier | true | values entering FiFo are marked as outlier/no outlier according to current FiFo's N(µ,σ)|
| 6 | mean with adaptive outlier detection | true | counted outlier vs. counted valid must hold a ratio, limit is in-/decreased accordingly: µ+/-limit|
| 7 | lin. regression (least square) | true | https://en.wikipedia.org/wiki/Linear_least_squares_(mathematics) |


| # | Type | FiFo size | outlier detection σ | pitch | performance |  
|---|------|-----------|---------------------|-------|-------------|
| 1 |      |           |                     |       |             |
