#!/usr/bin/env ruby

Dir.glob("bin/*").each do | file |
  f = `#{file}`
  puts f
end
