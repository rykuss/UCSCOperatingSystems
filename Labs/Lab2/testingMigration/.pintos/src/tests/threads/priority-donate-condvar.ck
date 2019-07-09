# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(priority-donate-condvar) begin
(priority-donate-condvar) Thread low acquired lock
(priority-donate-condvar) Thread main signaling
(priority-donate-condvar) Thread low woke up
(priority-donate-condvar) Thread high acquired lock
(priority-donate-condvar) Thread high signaling
(priority-donate-condvar) Thread high finished
(priority-donate-condvar) Thread med finished
(priority-donate-condvar) Thread low finished
(priority-donate-condvar) Thread main finished
(priority-donate-condvar) end
EOF
pass;