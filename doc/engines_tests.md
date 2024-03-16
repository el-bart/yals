# engines tests

sumary of all tested engines.

| type          | speed | stuck-at-end | stall current | running current | price   |
|---------------|-------|--------------|---------------|-----------------|---------|
| MT83          | good  | yes :(       | 2 A           | 0.7 A           | 1.5 EUR |
| MT44          | good  | yes :(       | 0.6 A         | 0.25 A          | 3.7 EUR |
| MT78          | good  | yes :(       | 0.8 A         | 0.3 A           | 1.6 EUR |
| johnson 10435 | slow  | yes :(       | 0.2 A         | 0.1 A           | 1.6 EUR |

inital model (v0.1) was prepared for MT83 engine.
it is however too power-hungry, especially at stall.

johnson 10435 is far too weak for this application.
it is rejected.

MT44 and MT78 were performing very similar.
however MT44 is almost 2.5x more expensive.
therefor MT78 is the winner.
