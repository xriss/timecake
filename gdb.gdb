
define hook-quit
    set confirm off
end

target remote raspocd.local:3333

monitor reset halt

file out/timecake.out

load

