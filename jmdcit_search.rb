require 'ffi'
module Jmdict
  extend FFI::Library
  ffi_lib 'jmdict_dictionary'

  attach_function :load_with_ruby, [:string, :string, :string], :bool
end
