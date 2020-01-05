
define hook-quit
    set confirm off
end

target remote 10.42.0.203:3333

monitor reset halt

file out/src/timecake.out

load

