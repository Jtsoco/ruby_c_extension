require 'ffi'
require 'eiwa'
module Jmdict
  extend FFI::Library
  ffi_lib 'jmdict_dictionary'

  attach_function :load_with_ruby, [:string, :string, :string], :bool

  attach_function :check_english, [:string], :string

  attach_function :unload, [], :bool

  def load_the_words
    count = 0;
  Eiwa.parse_file("JMdict_e.xml", type: :jmdict_e) do |entry|
    break if count > 25
    puts "generating word"
    dictionary_line = []
    dictionary_line << entry.text
    puts dictionary_line.last
    dictionary_line << entry.meanings.first.definitions.first.text
    puts dictionary_line.last
    dictionary_line << entry.readings.first.text
    puts dictionary_line.last
    dictionary_line
    puts "word saved!"
    Jmdict.load_with_ruby(dictionary_line[0], dictionary_line[1], dictionary_line[2])
    puts "loaded word!"
    count += 1
    end
    puts "loaded full dictionary"
  end
end
load_the_words()
puts Jmdict.check_english('明かん')
if (Jmdict.unload)
  puts "Dictionary closed!"
end
