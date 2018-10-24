###############################################################################
# This file is part of the TouchGFX-std 4.9.3 distribution.
# Copyright (C) 2017 Draupner Graphics A/S <http://www.touchgfx.com>.
###############################################################################
# This is licensed software. Any use hereof is restricted by and subject to 
# the applicable license terms. For further information see "About/Legal
# Notice" in TouchGFX Designer or in your TouchGFX installation directory.
###############################################################################
class TypedTextDatabaseHpp < Template
  def input_path
    File.join(root_dir,'Templates','TypedTextDatabase.hpp.temp')
  end
  def output_path
    'include/texts/TypedTextDatabase.hpp'
  end
end
