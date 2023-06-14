require 'ffi'
module A
  extend FFI::Library
  ffi_lib 'foobar'

  attach_function :foobar, [:int], :int
  attach_function :arr_hunt, [], :int
end

puts(A.foobar(3))
puts (A.arr_hunt)
# module A
#   extend FFI::Library
#   ffi_lib 'c'

#   attach_function :strlen, [:string], :int
# end

# puts(A.strlen("abc"))
# # 3
