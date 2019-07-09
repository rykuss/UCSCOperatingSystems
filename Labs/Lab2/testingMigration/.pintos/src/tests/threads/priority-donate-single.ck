# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(priority-donate-single) begin
(priority-donate-single) Thread B finished.
(priority-donate-single) Thread C finished.
(priority-donate-single) Thread A finished.
(priority-donate-single) end
EOF
pass;
