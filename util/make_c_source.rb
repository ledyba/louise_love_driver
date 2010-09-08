# -*- coding: utf-8 -*-

=begin
	make_c_source.rb
	libparser.rbで形態素解析した文章を用いて、
	louise_loveドライバで使うためのデータを生成するモジュールです。

	出力されたデータを、louise_love.cの該当部分にコピペしてください。

	usage: ruby make_c_source.rb <文章ファイル> > <出力ファイル>
=end
=begin
    "I love Louise tan!" Device Driver Util
    Copyright (C) 2010 PSI

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
=end

require './libparser.rb'


module Markov
class DB
	def initialize()
		@surface = [];
		@reading = [];
		@pos = [];
		@word = [];
		@word_unique = [];
	end
	def add(words)
		words.each{ |word|
			unless @surface.include? word.surface
				@surface.push(word.surface)
			end
			unless @reading.include? word.reading
				@reading.push(word.reading)
			end
			unless @pos.include? word.pos
				@pos.push(word.pos)
			end
			unless @word_unique.include? word
				@word_unique.push(word)
			end
			@word.push(word)
		}
	end
	def out()
		puts "#define CopipeSize (#{@word.size})"
		puts "static const struct Word Copipe[CopipeSize] = {"
		@word.each{ |word|
			puts "\t{.id=#{@word_unique.index(word)},\t.surface=#{@surface.index(word.surface)},\t.reading=#{@reading.index(word.reading)},\t.pos=#{@pos.index(word.pos)}\t},"
		}
		puts "};"

		puts "#define SurfaceSize (#{@surface.size})"
		puts "const char* Surface[SurfaceSize] = {"
		@surface.each{ |surface|
			puts "\t\"#{surface.to_s.gsub("\n","\\n")}\","
		}
		puts "};"

		puts "#define ReadingSize (#{@reading.size})"
		puts "const char* Reading[ReadingSize] = {"
		@reading.each{ |reading|
			puts "\t\"#{reading.to_s.gsub("\n","\\n")}\","
		}
		puts "};"

		puts "#define PosSize (#{@pos.size})"
		puts "const char* Pos[PosSize] = {"
		@pos.each{ |pos|
			puts "\t\"#{pos.to_s.gsub("\n","\\n")}\","
		}
		puts "};"


	end
end
end


def main(argv = ARGV)
	unless argv.size > 0
		$stderr.puts "usage: ruby make_c_source.rb <文章ファイル> > <出力ファイル>"
		return
	end
	parser = Markov::Parser::YahooParser.new();
	db = Markov::DB.new();
	argv.each(){|src|
		$stderr.puts "parsing: #{src}"
		file = open(src,'r:UTF-8');
		txt = file.read;
		txt.gsub!(/[\t\n 　]/,'')
		db.add(parser.parse(txt));
		file.close();
	}
	$stderr.puts "---"
	$stderr.puts "generating..."
	$stderr.puts "---"
	db.out();
	$stderr.puts "generated."
end

main();

