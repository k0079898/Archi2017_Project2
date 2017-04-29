lw    $a0, 0($0)
lw    $a1, 4($0)
addi  $t0, $a0, 8
sub   $t0, $a1, $a0
lw    $a2, 8($0)
lw    $a3, 12($0)
and   $v0, $a2, $a3
or    $zero, $t0, $t1
addi  $sp, $sp, -4
sw    $ra, 0($sp)
jal   0x0000
