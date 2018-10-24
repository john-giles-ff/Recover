###############################################################################
# This file is part of the TouchGFX-std 4.9.3 distribution.
# Copyright (C) 2017 Draupner Graphics A/S <http://www.touchgfx.com>.
###############################################################################
# This is licensed software. Any use hereof is restricted by and subject to 
# the applicable license terms. For further information see "About/Legal
# Notice" in TouchGFX Designer or in your TouchGFX installation directory.
###############################################################################
require 'roo'
require 'lib/text_entries'

class ExcelRow

  def initialize(excel_file, header, row_number, first_column)
    @excel_file = excel_file
    @header = header
    @row_number = row_number
    @first_column = first_column
  end

  def [](column_header)
    value_at(@row_number, column_number(column_header.to_s))
  end

  def exists?(name)
    column_index = @header.find_index { |val| val.downcase == name.to_s.downcase }
    !column_index.nil?
  end

  private

  def column_number(name)
    column_index = @header.find_index { |val| val.downcase == name.downcase }
    if column_index.nil?
      raise "#{name} column not found in excel file"
    end
    column_index + @first_column
  end

  def value_at(row, col)
    if @excel_file.empty?(row,col)
      nil
    else
      value = @excel_file.cell(row,col).to_s
      check_encoding(value)
      value
    end
  end

  def check_encoding(value)
    puts value if value.force_encoding("UTF-8").valid_encoding? == false
  end

end

class ExcelReader

  def initialize(file_name, sheet, header_row, first_column)
    @excel_file = Roo::Excelx.new file_name
    @sheet = sheet
    @excel_file.default_sheet = sheet
    @header_row = header_row
    @first_column = first_column
  end

  def read_header
    yield header
  end

  def read_rows
    (@header_row + 1).upto(last_row_number) do |row_number|
      yield row(row_number)
    end
  end

  private

  def last_row_number
    @excel_file.last_row
  end

  def header
    @excel_file.row(@header_row).compact.map(&:strip)
  end

  def row(row_number)
    ExcelRow.new(@excel_file, header, row_number, @first_column)
  end

end
