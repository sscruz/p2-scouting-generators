# p2-scouting-generators

This repository contains some event generators for the CMS Phase-2 Level-1 Data Scouting.

The formats are compatible with those used in https://github.com/gpetruc/p2-scouting-clients

The first set of those generates Puppi candidates, encoded as 64 bit words.
 * **gen4**: generates events with N candidates, sent out on 4 links, with usually 54 clock cycles per event (tmux period 6, 9 clocks per bx). 
       The generator is deterministic, it receives in input at each orbit the number of clock cycles per event and the bx offset of the first event,
       and list of number of puppi candidates per event for all the events of the orbit, and just generates dummy packets conforming to that lengths.
 * **genx4**: same format, but each event is prepended with an event header with the orbit number (received as input), bx numbers, and the number of candidates per event

Other tools:
 * **xoshiro256**: Generates 64-bit random numbers using an adaptation of [Xoshiro256**](https://en.wikipedia.org/wiki/Xorshift#xoshiro256**) 

