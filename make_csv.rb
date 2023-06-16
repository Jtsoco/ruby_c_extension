require 'csv'
require 'eiwa'
headers = ["Japanese", "English", "Reading"]
count = 0
CSV.open("medium_dictionary.csv", 'a+') do|csv|
  csv << headers if csv.count.eql? 0
  Eiwa.parse_file("JMdict_e.xml", type: :jmdict_e) do |entry|
    break if count > 10000
    puts "generating word"
    dictionary_line = []
    dictionary_line << entry.text
    puts dictionary_line.last
    dictionary_line << entry.meanings.first.definitions.first.text
    puts dictionary_line.last
    dictionary_line << entry.readings.first.text
    puts dictionary_line.last
    csv << dictionary_line
    puts "word saved!"
    count += 1
  end
end
puts("Total dictionary count: #{count} words")
# # gives us the meaning hash
# entry.meanings.first
# # gives array of all definitions
# entry.meanings.first.definitions
# # gives first of the definitions, there can be more than one, and it gives an instance
# entry.meanings.first.definitions.first
# # gives english characters, .text also works
# entry.meanings.first.definitions.first.characters
# # gives language (english only in this dicitonary?
# entry.meanings.first.definitions.first.language
# # gives a lot of other info
# entry.meanings.first.parent
# # gives japanese reading, furigana
# # entry.meanings.first.parent.readings.first.text
# entry.readings.first.text
# # gives the spelling with kanji might not always have array
# entry.spellings.first.text
# # gets misc tags, like idiomatic expression
# entry.meanings.first.misc_tags.first.text
# # returns an array of things, like if it's a phrase or a noun depending on the .text of the element
# meanings.first.parts_of_speech #.first.text, [1].text or whichever
# # gives the japanese
# entry.text
