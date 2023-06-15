require 'ffi'
require 'eiwa'
module Jmdict
  extend FFI::Library
  ffi_lib 'jmdict_dictionary'

  attach_function :load_with_ruby, [:string, :string, :string], :bool

  attach_function :check_english, [:string], :string

  attach_function :unload, [], :bool

  def self.load_the_words
    count = 0;
    Eiwa.parse_file("JMdict_e.xml", type: :jmdict_e) do |entry|
    break if count > 25
    # puts "generating word"
    dictionary_line = []
    dictionary_line << entry.text
    puts dictionary_line.last
    dictionary_line << entry.meanings.first.definitions.first.text
    puts dictionary_line.last
    dictionary_line << entry.readings.first.text
    puts dictionary_line.last
    dictionary_line
    puts "word saved!"
    puts count
    if (!dictionary_line.any?(nil) || count != 13 || count != 12 || count != 14 || count != 17)
      puts "Loading word!"
      Jmdict.load_with_ruby(dictionary_line[0], dictionary_line[1], dictionary_line[2])
      puts "loaded word!"
      count += 1
      puts "Total words in dictionary: " + count.to_s
      end
    end
    puts "loaded full dictionary"
  end
end
Jmdict.load_the_words()
puts "Here's teh english word!"
puts Jmdict.check_english('ＣＤプレーヤー')
puts "That was it!"
if (Jmdict.unload)
  puts "Dictionary closed!"
end
