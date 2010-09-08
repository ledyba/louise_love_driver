# -*- coding: utf-8 -*-

# 自分の取得したIDをいれてね。
# 登録は
# https://e.developer.yahoo.co.jp/webservices/register_application
# でできます。
APP_ID = ""

=begin
	libparser.rb
	Yahoo!の形態素解析サービスを利用して、文章を形態素解析するモジュールです。
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

require 'rexml/document'
require 'net/http'

if APP_ID.empty?
	raise "parser.rbの先頭にあるAPI_IDを設定してください。";
end

module Markov
module Parser

class Text
	def initialize()
		@words = Array.new();
	end
	def << (item)
		@words << item;
	end
	def each(&block)
		@words.each(&block);
	end
	def [](num)
		return @words[num]
	end
	def size()
		return @words.size;
	end
	def each(&block)
		@words.each(&block);
	end
end

class Word
	def initialize(surface,reading,pos,baseform)
		@surface = surface;
		@reading = reading;
		@pos = pos.to_sym;
		@baseform = baseform;
		@hash_code = surface.hash + reading.hash + pos.hash + baseform.hash;
	end
	def to_s()
		ret = "{"
		ret << "表記:#{@surface}," if @surface != nil
		ret << "よみがな:#{@reading}," if @reading != nil
		ret << "品詞:#{pos}," if @pos != nil
		ret << "終止形:#{baseform}," if @baseform != nil
		ret << "}"
		return ret;
	end
	def hash
		return @hash_code;
	end
	def eql?(obj)
		unless obj.is_a? Word
			return false;
		end
		return @hash_code == obj.hash_code &&
			@surface == obj.surface &&
			@reading == obj.reading &&
			@pos == obj.pos &&
			@baseform == obj.baseform;
	end
	def ==(obj)
		return eql?(obj)
	end

	attr_reader :surface,:reading,:pos,:baseform,:hash_code
end

class Parser
end

class YahooParser < Parser
	API_SERVER = 'jlp.yahooapis.jp'
	API_URL = '/MAService/V1/parse';
	BOM_UTF8 = [0xef, 0xbb, 0xbf].pack("c3")
	def initialize(api_id = APP_ID)
		@data = "appid=#{api_id}&results=ma&sentence="
	end
	def parse(text)
		http = Net::HTTP.new(API_SERVER);
		resp = http.post(API_URL,@data+URI.encode("#{text}"));
		if resp.code != '200'
			raise "Yahoo! Japan Parser unavailable. Return Code:#{resp.code}"
		end
		body = resp.body;
		doc = REXML::Document.new(body).elements['ResultSet/ma_result/word_list/'];
		text = Text.new();
		doc.elements.each('word') {|item|
			surface = item.elements['surface'].text if item.elements['surface'] != nil
			next if surface.empty? || surface == BOM_UTF8
			reading = item.elements['reading'].text if item.elements['reading'] != nil
			pos = item.elements['pos'].text if item.elements['pos'] != nil
			baseform = item.elements['baseform'].text if item.elements['baseform'] != nil
			text << Word.new(surface,reading,pos,baseform)
		}
		return text;
	end
end
end
end
