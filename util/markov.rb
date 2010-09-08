# -*- coding: utf-8 -*-﻿
=begin
	markov.rb
	libparser.rbで形態素解析した文章を用いて、実際にマルコフ連鎖して文章を作成するモジュールです。

	usage: ruby marcov.rb <文章ファイル>
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
class Dic
	def initialize()
		@dic = Hash.new();
		@dic2 = Hash.new();
		@size = 0;
	end
	def add(text)
		if @first == nil
			@first = text[0]
			@second = text[1]
		end
		#二階
		for i in 0..text.size-2
			word = text[i]
			if @dic[word.reading] == nil
				@dic[word.reading] = Array.new
			end
			@dic[word.reading] << text[i+1]
			@size += 1;
		end
		#三階
		for i in 0..text.size-3
			word = text[i]
			word2 = text[i+1]
			if @dic2[word.reading] == nil
				@dic2[word.reading] = Hash.new
			end
			if @dic2[word.reading][word2.reading] == nil
				@dic2[word.reading][word2.reading] = Array.new
			end
			@dic2[word.reading][word2.reading] << text[i+2]
#			puts "#{word.reading}(#{word.pos})+#{word2.reading}(#{word2.pos})->#{text[i+2].reading}"
		end
	end
	attr_reader :size
	def next(word = nil,word2 = nil)
		# 三階の連鎖
		if word != nil && word2 != nil
=begin
			# 話題を強制的に変える
			if word2.pos == :特殊
				array = @dic[word2.reading];
				return array[rand(array.length)];
			end
=end
			#通常の三階連鎖
			array = @dic2[word.reading][word2.reading];
			if array == nil #辞書に入っていない場合は、二階のマルコフ連鎖を行う
				array = @dic[word2.reading];
			end
			return array[rand(array.length)];
		end
		# 最初
		if word == nil && word2 == nil
			return @first
		end
		# どちらかnullでない方
		left = (word == nil ? word2 : word);
		# 次
		if left == @first
			return @second
		end
		# どっちかが足りない＝二階の連鎖を行わざるをえない
		array = @dic[left.reading];
		return array[rand(array.length)];
	end
end
class Generator
	SENT_END=/^(.+?)([.．。]|[!！？]+)$/
	def initialize(dic)
		@dic = dic;
	end
	def generate(word=nil,length = 200)
		word = @dic.next() if word == nil
		word0 = nil
		ret = ""
		cnt = 0;
		#長さがある程度を超えるか、文章の終わりまでいったら終わり。
		while cnt < length || !(cnt >= length+20 || (SENT_END.match(ret) && word.pos != :特殊))
			ret << word.surface;
			new_word = @dic.next(word0,word);
			word0 = word
			word = new_word
			cnt+=1;
		end
		return ret
	end
end
end

def main(argv = ARGV)
	unless argv.size > 0
		puts "usage: ruby marcov.rb <文章ファイル>"
		return
	end
	parser = Markov::Parser::YahooParser.new();
	dic = Markov::Dic.new();
	argv.each(){|src|
		puts "parsing: #{src}"
		file = open(src,'r:UTF-8');
		txt = file.read;
		txt.gsub!(/[\t\n 　]/,'')
		dic.add(parser.parse(txt))
		file.close();
	}
	puts "---"
	puts "generating"
	marcov = Markov::Generator.new(dic);
	for i in 0..5
		puts "---"
		puts marcov.generate
	end
end

main();
