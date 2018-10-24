###############################################################################
# This file is part of the TouchGFX-std 4.9.3 distribution.
# Copyright (C) 2017 Draupner Graphics A/S <http://www.touchgfx.com>.
###############################################################################
# This is licensed software. Any use hereof is restricted by and subject to 
# the applicable license terms. For further information see "About/Legal
# Notice" in TouchGFX Designer or in your TouchGFX installation directory.
###############################################################################
class UnicodesTxt
  def initialize(text_entries, typographies, output_directory)
    @text_entries = text_entries
    @typographies = typographies
    @output_directory = output_directory
  end
  def run
    unique_typographies = @typographies.map{ |t| Typography.new("", t.font_file, t.font_size, t.bpp) }.uniq.sort_by { |t| sprintf("%s %04d %d",t.font_file,t.font_size,t.bpp) }
    unique_typographies.each do |unique_typography|
      UnicodeForTypographyTxt.new(@text_entries, @output_directory, @typographies, unique_typography).run
    end
  end
end

class UnicodeForTypographyTxt
  def initialize(text_entries, output_directory, typographies, unique_typography)
    @text_entries = text_entries
    @output_directory = output_directory
    @typographies = typographies
    @unique_typography = unique_typography
  end
  def convert_to_contextual_forms(unicodes)
    @contextutal_forms ||= { 0x0621=>[0xFE80],
                             0x0622=>[0xFE81,0xFE82,0xFEF5,0xFEF6,0xFEFF],
                             0x0623=>[0xFE83,0xFE84,0xFEF7,0xFEF8,0xFEFF],
                             0x0624=>[0xFE85,0xFE86],
                             0x0625=>[0xFE87,0xFE88,0xFEF9,0xFEFA],
                             0x0626=>[0xFBEA,0xFBEB,0xFBEC,0xFBED,0xFBEE,0xFBEF,0xFBF0,0xFBF1,0xFBF2,0xFBF3,0xFBF4,0xFBF5,0xFBF6,0xFBF7,0xFBF8,0xFC00,0xFC01,0xFC02,0xFC03,0xFC04,0xFC64,0xFC65,0xFC66,0xFC67,0xFC68,0xFC69,0xFC97,0xFC98,0xFC99,0xFC9A,0xFC9B,0xFCDF,0xFCE0,0xFE89,0xFE8A,0xFE8B,0xFE8C,0xFEFF],
                             0x0627=>[0xFBEA,0xFBEB,0xFC37,0xFC80,0xFC88,0xFD3C,0xFD3D,0xFDF2,0xFDF3,0xFDFA,0xFDFB,0xFDFC,0xFE8D,0xFE8E,0xFEFB,0xFEFC,0xFEFF],
                             0x0628=>[0xFC05,0xFC06,0xFC07,0xFC08,0xFC09,0xFC0A,0xFC6A,0xFC6B,0xFC6C,0xFC6D,0xFC6E,0xFC6F,0xFC9C,0xFC9D,0xFC9E,0xFC9F,0xFCA0,0xFCE1,0xFCE2,0xFD9E,0xFDC2,0xFDF3,0xFE8F,0xFE90,0xFE91,0xFE92,0xFEFF],
                             0x0629=>[0xFE93,0xFE94],
                             0x062A=>[0xFC0B,0xFC0C,0xFC0D,0xFC0E,0xFC0F,0xFC10,0xFC70,0xFC71,0xFC72,0xFC73,0xFC74,0xFC75,0xFCA1,0xFCA2,0xFCA3,0xFCA4,0xFCA5,0xFCE3,0xFCE4,0xFD50,0xFD51,0xFD52,0xFD53,0xFD54,0xFD55,0xFD56,0xFD57,0xFD9F,0xFDA0,0xFDA1,0xFDA2,0xFDA3,0xFDA4,0xFE95,0xFE96,0xFE97,0xFE98,0xFEFF],
                             0x062B=>[0xFC11,0xFC12,0xFC13,0xFC14,0xFC76,0xFC77,0xFC78,0xFC79,0xFC7A,0xFC7B,0xFCA6,0xFCE5,0xFCE6,0xFE99,0xFE9A,0xFE9B,0xFE9C,0xFEFF],
                             0x062C=>[0xFC00,0xFC05,0xFC0B,0xFC11,0xFC15,0xFC16,0xFC17,0xFC19,0xFC1C,0xFC22,0xFC29,0xFC2B,0xFC2D,0xFC38,0xFC3F,0xFC45,0xFC4B,0xFC51,0xFC55,0xFC97,0xFC9C,0xFCA1,0xFCA7,0xFCA8,0xFCA9,0xFCAB,0xFCAD,0xFCB4,0xFCBA,0xFCBC,0xFCBE,0xFCC4,0xFCC9,0xFCCE,0xFCD2,0xFCD7,0xFCDA,0xFD01,0xFD02,0xFD09,0xFD1D,0xFD1E,0xFD25,0xFD2D,0xFD34,0xFD37,0xFD50,0xFD51,0xFD52,0xFD55,0xFD58,0xFD59,0xFD5C,0xFD5D,0xFD5E,0xFD61,0xFD69,0xFD75,0xFD83,0xFD84,0xFD89,0xFD8C,0xFD8D,0xFD8E,0xFD92,0xFD93,0xFD97,0xFD98,0xFD99,0xFD9F,0xFDA0,0xFDA5,0xFDA6,0xFDA7,0xFDAC,0xFDAF,0xFDB8,0xFDBA,0xFDBC,0xFDBD,0xFDBE,0xFDBF,0xFDC0,0xFDC4,0xFDC7,0xFDFB,0xFE9D,0xFE9E,0xFE9F,0xFEA0,0xFEFF],
                             0x062D=>[0xFC01,0xFC06,0xFC0C,0xFC15,0xFC17,0xFC18,0xFC1A,0xFC1D,0xFC20,0xFC23,0xFC26,0xFC2E,0xFC33,0xFC39,0xFC40,0xFC46,0xFC4C,0xFC56,0xFC98,0xFC9D,0xFCA2,0xFCA7,0xFCA9,0xFCAA,0xFCAE,0xFCB1,0xFCB5,0xFCB8,0xFCBF,0xFCC2,0xFCC5,0xFCCA,0xFCCF,0xFCD3,0xFCDB,0xFCFF,0xFD00,0xFD0A,0xFD1B,0xFD1C,0xFD26,0xFD2E,0xFD35,0xFD38,0xFD51,0xFD52,0xFD53,0xFD56,0xFD58,0xFD59,0xFD5A,0xFD5B,0xFD5C,0xFD5D,0xFD5F,0xFD60,0xFD64,0xFD65,0xFD67,0xFD68,0xFD6E,0xFD71,0xFD72,0xFD7E,0xFD80,0xFD81,0xFD82,0xFD87,0xFD88,0xFD89,0xFD8A,0xFD8B,0xFD8C,0xFD95,0xFD96,0xFDA6,0xFDA9,0xFDAA,0xFDAB,0xFDAE,0xFDB3,0xFDB4,0xFDB5,0xFDB8,0xFDBD,0xFDBE,0xFDBF,0xFDC2,0xFDF4,0xFEA1,0xFEA2,0xFEA3,0xFEA4,0xFEFF],
                             0x062E=>[0xFC07,0xFC0D,0xFC19,0xFC1A,0xFC1B,0xFC1E,0xFC24,0xFC2F,0xFC3A,0xFC41,0xFC47,0xFC4D,0xFC57,0xFC99,0xFC9E,0xFCA3,0xFCAB,0xFCAC,0xFCAF,0xFCB2,0xFCB6,0xFCC0,0xFCC6,0xFCCB,0xFCD0,0xFCD4,0xFCDC,0xFD03,0xFD04,0xFD0B,0xFD1F,0xFD20,0xFD27,0xFD2F,0xFD36,0xFD39,0xFD54,0xFD57,0xFD6A,0xFD6B,0xFD6F,0xFD70,0xFD7C,0xFD7D,0xFD85,0xFD86,0xFD8E,0xFD8F,0xFD92,0xFD9E,0xFDA1,0xFDA2,0xFDA8,0xFDB9,0xFDC6,0xFEA5,0xFEA6,0xFEA7,0xFEA8,0xFEFF],
                             0x062F=>[0xFDF4,0xFEA9,0xFEAA,0xFEFF],
                             0x0630=>[0xFC5B,0xFEAB,0xFEAC,0xFEFF],
                             0x0631=>[0xFC5C,0xFC64,0xFC6A,0xFC70,0xFC76,0xFC8A,0xFC91,0xFD0D,0xFD0E,0xFD0F,0xFD10,0xFD29,0xFD2A,0xFD2B,0xFD2C,0xFDF3,0xFDF6,0xFDFC,0xFEAD,0xFEAE,0xFEFF],
                             0x0632=>[0xFC65,0xFC6B,0xFC71,0xFC77,0xFC8B,0xFC92,0xFEAF,0xFEB0,0xFEFF],
                             0x0633=>[0xFC1C,0xFC1D,0xFC1E,0xFC1F,0xFCAD,0xFCAE,0xFCAF,0xFCB0,0xFCE7,0xFCE8,0xFCFB,0xFCFC,0xFD0E,0xFD17,0xFD18,0xFD2A,0xFD31,0xFD34,0xFD35,0xFD36,0xFD5C,0xFD5D,0xFD5E,0xFD5F,0xFD60,0xFD61,0xFD62,0xFD63,0xFDA8,0xFDC6,0xFDF6,0xFDF8,0xFDFA,0xFEB1,0xFEB2,0xFEB3,0xFEB4,0xFEFF],
                             0x0634=>[0xFCE9,0xFCEA,0xFCFD,0xFCFE,0xFD09,0xFD0A,0xFD0B,0xFD0C,0xFD0D,0xFD19,0xFD1A,0xFD25,0xFD26,0xFD27,0xFD28,0xFD29,0xFD2D,0xFD2E,0xFD2F,0xFD30,0xFD32,0xFD37,0xFD38,0xFD39,0xFD67,0xFD68,0xFD69,0xFD6A,0xFD6B,0xFD6C,0xFD6D,0xFDAA,0xFEB5,0xFEB6,0xFEB7,0xFEB8,0xFEFF],
                             0x0635=>[0xFC20,0xFC21,0xFCB1,0xFCB2,0xFCB3,0xFD05,0xFD06,0xFD0F,0xFD21,0xFD22,0xFD2B,0xFD64,0xFD65,0xFD66,0xFDA9,0xFDC5,0xFDF0,0xFDF5,0xFDF9,0xFDFA,0xFEB9,0xFEBA,0xFEBB,0xFEBC,0xFEFF],
                             0x0636=>[0xFC22,0xFC23,0xFC24,0xFC25,0xFCB4,0xFCB5,0xFCB6,0xFCB7,0xFD07,0xFD08,0xFD10,0xFD23,0xFD24,0xFD2C,0xFD6E,0xFD6F,0xFD70,0xFDAB,0xFEBD,0xFEBE,0xFEBF,0xFEC0,0xFEFF],
                             0x0637=>[0xFC26,0xFC27,0xFCB8,0xFCF5,0xFCF6,0xFD11,0xFD12,0xFD33,0xFD3A,0xFD71,0xFD72,0xFD73,0xFD74,0xFEC1,0xFEC2,0xFEC3,0xFEC4,0xFEFF],
                             0x0638=>[0xFC28,0xFCB9,0xFD3B,0xFEC5,0xFEC6,0xFEC7,0xFEC8,0xFEFF],
                             0x0639=>[0xFC29,0xFC2A,0xFCBA,0xFCBB,0xFCF7,0xFCF8,0xFD13,0xFD14,0xFD75,0xFD76,0xFD77,0xFD78,0xFDB6,0xFDC4,0xFDF5,0xFDF7,0xFDFA,0xFEC9,0xFECA,0xFECB,0xFECC,0xFEFF],
                             0x063A=>[0xFC2B,0xFC2C,0xFCBC,0xFCBD,0xFCF9,0xFCFA,0xFD15,0xFD16,0xFD79,0xFD7A,0xFD7B,0xFECD,0xFECE,0xFECF,0xFED0,0xFEFF],
                             0x0640=>[0x0640,0xFCF2,0xFCF3,0xFCF4,0xFE71,0xFE77,0xFE79,0xFE7B,0xFE7D,0xFE7F,0xFEFF],
                             0x0641=>[0xFC2D,0xFC2E,0xFC2F,0xFC30,0xFC31,0xFC32,0xFC7C,0xFC7D,0xFCBE,0xFCBF,0xFCC0,0xFCC1,0xFD7C,0xFD7D,0xFDC1,0xFED1,0xFED2,0xFED3,0xFED4,0xFEFF],
                             0x0642=>[0xFC33,0xFC34,0xFC35,0xFC36,0xFC7E,0xFC7F,0xFCC2,0xFCC3,0xFD7E,0xFD7F,0xFDB2,0xFDB4,0xFDF1,0xFED5,0xFED6,0xFED7,0xFED8,0xFEFF],
                             0x0643=>[0xFC37,0xFC38,0xFC39,0xFC3A,0xFC3B,0xFC3C,0xFC3D,0xFC3E,0xFC80,0xFC81,0xFC82,0xFC83,0xFC84,0xFCC4,0xFCC5,0xFCC6,0xFCC7,0xFCC8,0xFCEB,0xFCEC,0xFDB7,0xFDBB,0xFDC3,0xFDF3,0xFED9,0xFEDA,0xFEDB,0xFEDC,0xFEFF],
                             0x0644=>[0xFC3B,0xFC3F,0xFC40,0xFC41,0xFC42,0xFC43,0xFC44,0xFC81,0xFC85,0xFC86,0xFC87,0xFCC7,0xFCC9,0xFCCA,0xFCCB,0xFCCC,0xFCCD,0xFCEB,0xFCED,0xFD80,0xFD81,0xFD82,0xFD83,0xFD84,0xFD85,0xFD86,0xFD87,0xFD88,0xFDAC,0xFDAD,0xFDB5,0xFDBA,0xFDBC,0xFDF0,0xFDF1,0xFDF2,0xFDF5,0xFDF6,0xFDF7,0xFDF8,0xFDF9,0xFDFA,0xFDFB,0xFDFC,0xFEDD,0xFEDE,0xFEDF,0xFEE0,0xFEF5,0xFEF6,0xFEF7,0xFEF8,0xFEF9,0xFEFA,0xFEFB,0xFEFC,0xFEFF],
                             0x0645=>[0xFC02,0xFC08,0xFC0E,0xFC12,0xFC16,0xFC18,0xFC1B,0xFC1F,0xFC21,0xFC25,0xFC27,0xFC28,0xFC2A,0xFC2C,0xFC30,0xFC34,0xFC3C,0xFC42,0xFC45,0xFC46,0xFC47,0xFC48,0xFC49,0xFC4A,0xFC4E,0xFC52,0xFC58,0xFC66,0xFC6C,0xFC72,0xFC78,0xFC82,0xFC85,0xFC88,0xFC89,0xFC8C,0xFC93,0xFC9A,0xFC9F,0xFCA4,0xFCA6,0xFCA8,0xFCAA,0xFCAC,0xFCB0,0xFCB3,0xFCB7,0xFCB9,0xFCBB,0xFCBD,0xFCC1,0xFCC3,0xFCC8,0xFCCC,0xFCCE,0xFCCF,0xFCD0,0xFCD1,0xFCD5,0xFCD8,0xFCDD,0xFCDF,0xFCE1,0xFCE3,0xFCE5,0xFCE7,0xFCE9,0xFCEC,0xFCED,0xFCEE,0xFCF0,0xFD0C,0xFD28,0xFD30,0xFD33,0xFD3A,0xFD3B,0xFD50,0xFD53,0xFD54,0xFD55,0xFD56,0xFD57,0xFD58,0xFD59,0xFD5A,0xFD5B,0xFD5F,0xFD60,0xFD61,0xFD62,0xFD63,0xFD66,0xFD67,0xFD68,0xFD6A,0xFD6B,0xFD6C,0xFD6D,0xFD6F,0xFD70,0xFD71,0xFD72,0xFD73,0xFD74,0xFD75,0xFD76,0xFD77,0xFD78,0xFD79,0xFD7A,0xFD7B,0xFD7C,0xFD7D,0xFD7E,0xFD7F,0xFD80,0xFD85,0xFD86,0xFD87,0xFD88,0xFD89,0xFD8A,0xFD8B,0xFD8C,0xFD8D,0xFD8E,0xFD8F,0xFD92,0xFD93,0xFD94,0xFD95,0xFD97,0xFD98,0xFD9A,0xFD9B,0xFD9C,0xFD9D,0xFDA3,0xFDA4,0xFDA5,0xFDA7,0xFDAD,0xFDB0,0xFDB1,0xFDB2,0xFDB4,0xFDB5,0xFDB6,0xFDB7,0xFDB9,0xFDBA,0xFDBB,0xFDBC,0xFDC0,0xFDC1,0xFDC3,0xFDC4,0xFDC5,0xFDF4,0xFDF5,0xFDF8,0xFDFA,0xFEE1,0xFEE2,0xFEE3,0xFEE4,0xFEFF],
                             0x0646=>[0xFC4B,0xFC4C,0xFC4D,0xFC4E,0xFC4F,0xFC50,0xFC67,0xFC6D,0xFC73,0xFC79,0xFC8A,0xFC8B,0xFC8C,0xFC8D,0xFC8E,0xFC8F,0xFC94,0xFCD2,0xFCD3,0xFCD4,0xFCD5,0xFCD6,0xFCEE,0xFCEF,0xFD95,0xFD96,0xFD97,0xFD98,0xFD99,0xFD9A,0xFD9B,0xFDB3,0xFDB8,0xFDBD,0xFDC7,0xFEE5,0xFEE6,0xFEE7,0xFEE8,0xFEFF],
                             0x0647=>[0xFC51,0xFC52,0xFC53,0xFC54,0xFC9B,0xFCA0,0xFCA5,0xFCCD,0xFCD6,0xFCD7,0xFCD8,0xFCD9,0xFCDE,0xFCE0,0xFCE2,0xFCE4,0xFCE6,0xFCE8,0xFCEA,0xFCEF,0xFCF1,0xFD31,0xFD32,0xFD93,0xFD94,0xFDF2,0xFDF7,0xFDFA,0xFDFB,0xFEE9,0xFEEA,0xFEEB,0xFEEC,0xFEFF],
                             0x0648=>[0xFBEE,0xFBEF,0xFDF6,0xFDF8,0xFDFA,0xFEED,0xFEEE,0xFEFF],
                             0x0649=>[0xFBE8,0xFBE9,0xFC03,0xFC09,0xFC0F,0xFC13,0xFC31,0xFC35,0xFC3D,0xFC43,0xFC49,0xFC4F,0xFC53,0xFC59,0xFC5D,0xFC68,0xFC6E,0xFC74,0xFC7A,0xFC7C,0xFC7E,0xFC83,0xFC86,0xFC8E,0xFC90,0xFC95,0xFCF5,0xFCF7,0xFCF9,0xFCFB,0xFCFD,0xFCFF,0xFD01,0xFD03,0xFD05,0xFD07,0xFD11,0xFD13,0xFD15,0xFD17,0xFD19,0xFD1B,0xFD1D,0xFD1F,0xFD21,0xFD23,0xFD5B,0xFD5E,0xFD6E,0xFD78,0xFD7B,0xFD82,0xFD96,0xFD99,0xFD9B,0xFDA0,0xFDA2,0xFDA4,0xFDA6,0xFDA7,0xFDA8,0xFDF9,0xFDFA,0xFEEF,0xFEF0,0xFEFF],
                             0x064A=>[0xFC04,0xFC0A,0xFC10,0xFC14,0xFC32,0xFC36,0xFC3E,0xFC44,0xFC4A,0xFC50,0xFC54,0xFC55,0xFC56,0xFC57,0xFC58,0xFC59,0xFC5A,0xFC69,0xFC6F,0xFC75,0xFC7B,0xFC7D,0xFC7F,0xFC84,0xFC87,0xFC8F,0xFC91,0xFC92,0xFC93,0xFC94,0xFC95,0xFC96,0xFCDA,0xFCDB,0xFCDC,0xFCDD,0xFCDE,0xFCF0,0xFCF1,0xFCF6,0xFCF8,0xFCFA,0xFCFC,0xFCFE,0xFD00,0xFD02,0xFD04,0xFD06,0xFD08,0xFD12,0xFD14,0xFD16,0xFD18,0xFD1A,0xFD1C,0xFD1E,0xFD20,0xFD22,0xFD24,0xFD5A,0xFD69,0xFD74,0xFD7A,0xFD81,0xFD8B,0xFD9A,0xFD9C,0xFD9D,0xFD9E,0xFD9F,0xFDA1,0xFDA3,0xFDA5,0xFDA9,0xFDAA,0xFDAB,0xFDAC,0xFDAD,0xFDAE,0xFDAF,0xFDB0,0xFDB1,0xFDB2,0xFDB3,0xFDB6,0xFDB7,0xFDB9,0xFDBE,0xFDBF,0xFDC0,0xFDC1,0xFDC2,0xFDC6,0xFDC7,0xFDF7,0xFDFA,0xFEF1,0xFEF2,0xFEF3,0xFEF4,0xFEFF],
                             0x064B=>[0x064B,0xFD3C,0xFD3D,0xFE70,0xFE71,0xFEFF],
                             0x064C=>[0x064C,0xFC5E,0xFE72,0xFEFF],
                             0x064D=>[0x064D,0xFC5F,0xFE74,0xFEFF],
                             0x064E=>[0x064E,0xFC60,0xFCF2,0xFE76,0xFE77,0xFEFF],
                             0x064F=>[0x064F,0xFC61,0xFCF3,0xFE78,0xFE79,0xFEFF],
                             0x0650=>[0x0650,0xFC62,0xFCF4,0xFE7A,0xFE7B,0xFEFF],
                             0x0651=>[0x0651,0xFC5E,0xFC5F,0xFC60,0xFC61,0xFC62,0xFC63,0xFCF2,0xFCF3,0xFCF4,0xFE7C,0xFE7D,0xFEFF],
                             0x0652=>[0x0652,0xFE7E,0xFE7F],
                             0x0670=>[0x0670,0xFC5B,0xFC5C,0xFC5D,0xFC63,0xFC90,0xFCD9,0xFEFF],
                             0x0671=>[0xFB50,0xFB51],
                             0x0677=>[0xFBDD],
                             0x0679=>[0xFB66,0xFB67,0xFB68,0xFB69],
                             0x067A=>[0xFB5E,0xFB5F,0xFB60,0xFB61],
                             0x067B=>[0xFB52,0xFB53,0xFB54,0xFB55],
                             0x067E=>[0xFB56,0xFB57,0xFB58,0xFB59],
                             0x067F=>[0xFB62,0xFB63,0xFB64,0xFB65],
                             0x0680=>[0xFB5A,0xFB5B,0xFB5C,0xFB5D],
                             0x0683=>[0xFB76,0xFB77,0xFB78,0xFB79],
                             0x0684=>[0xFB72,0xFB73,0xFB74,0xFB75],
                             0x0686=>[0xFB7A,0xFB7B,0xFB7C,0xFB7D],
                             0x0687=>[0xFB7E,0xFB7F,0xFB80,0xFB81],
                             0x0688=>[0xFB88,0xFB89],
                             0x068C=>[0xFB84,0xFB85],
                             0x068D=>[0xFB82,0xFB83],
                             0x068E=>[0xFB86,0xFB87],
                             0x0691=>[0xFB8C,0xFB8D],
                             0x0698=>[0xFB8A,0xFB8B],
                             0x06A4=>[0xFB6A,0xFB6B,0xFB6C,0xFB6D],
                             0x06A6=>[0xFB6E,0xFB6F,0xFB70,0xFB71],
                             0x06A9=>[0xFB8E,0xFB8F,0xFB90,0xFB91],
                             0x06AD=>[0xFBD3,0xFBD4,0xFBD5,0xFBD6],
                             0x06AF=>[0xFB92,0xFB93,0xFB94,0xFB95],
                             0x06B1=>[0xFB9A,0xFB9B,0xFB9C,0xFB9D],
                             0x06B3=>[0xFB96,0xFB97,0xFB98,0xFB99],
                             0x06BA=>[0xFB9E,0xFB9F],
                             0x06BB=>[0xFBA0,0xFBA1,0xFBA2,0xFBA3],
                             0x06BE=>[0xFBAA,0xFBAB,0xFBAC,0xFBAD],
                             0x06C0=>[0xFBA4,0xFBA5],
                             0x06C1=>[0xFBA6,0xFBA7,0xFBA8,0xFBA9],
                             0x06C5=>[0xFBE0,0xFBE1],
                             0x06C6=>[0xFBD9,0xFBDA,0xFBF2,0xFBF3],
                             0x06C7=>[0xFBD7,0xFBD8,0xFBF0,0xFBF1,0xFEFF],
                             0x06C8=>[0xFBDB,0xFBDC,0xFBF4,0xFBF5,0xFEFF],
                             0x06C9=>[0xFBE2,0xFBE3],
                             0x06CB=>[0xFBDE,0xFBDF],
                             0x06CC=>[0xFBFC,0xFBFD,0xFBFE,0xFBFF,0xFDFC],
                             0x06D0=>[0xFBE4,0xFBE5,0xFBE6,0xFBE7,0xFBF6,0xFBF7,0xFBF8,0xFEFF],
                             0x06D2=>[0xFBAE,0xFBAF,0xFDF0,0xFDF1,0xFEFF],
                             0x06D3=>[0xFBB0,0xFBB1] }
    unicodes.collect do |u|
      @contextutal_forms.keys.include?(u) ? @contextutal_forms[u] : u
    end.flatten
  end
  def mirror_brackes(unicodes)
    [['<','>'],['(',')'],['[',']'],['{','}']].each do |l,r|
      has_l = unicodes.include?(l.ord)
      has_r = unicodes.include?(r.ord)
      unicodes.push(r.ord) if has_l and !has_r
      unicodes.push(l.ord) if has_r and !has_l
    end
    unicodes
  end
  def check_for_rtl(unicodes)
    return if @text_entries.is_rtl # No need to look for unicode if RTL already detected
    # Look for hebrew (0x0500-0x05ff) or arabic (0x0600-0x06ff) + arabic ligatures (0xFE70-0xFEFF)
    @text_entries.unicode_uses_rtl if unicodes.any?{|u| u.between?(0x0500, 0x05FF) || u.between?(0x0600, 0x06FF) || u.between?(0xFE70, 0xFEFF) }
  end
  def decode_ranges(str)
    result = []
    while str.length > 0
      char_range = str.match(/^(.)-(.)(.*)$/)
      if char_range
        first_char = char_range[1]
        last_char = char_range[2]
        result += (first_char.ord .. last_char.ord).to_a
        str = char_range[3]
      else
        num_range = str.match(/^(0[xX][0-9a-fA-F]+|\d+)(?:\.0+)?-(0[xX][0-9a-fA-F]+|\d+)(?:\.0+)?(.*)$/)
        if num_range
          first_num = Integer(num_range[1])
          last_num = Integer(num_range[2])
          result += (first_num..last_num).to_a
          str = num_range[3]
        else
          num = str.match(/^(0[xX][0-9a-fA-F]+|\d+)(?:\.0+)?(.*)/)
          if num
            # Check for typo such as 0,2-9
            if num[1].length == 1
              result += [ num[1].ord ]
            else
              result += [ Integer(num[1]) ]
            end
            str = num[2]
          else
            abort "Unexpected character at #{str}"
          end
        end
      end
      if str.length > 0
        if str[0] == ','
          str = str[1..-1]
        else
          abort "Please separate wildcard ranges with ','"
        end
      end
    end
    result
  end

  def run
    typographies_identical = @typographies.select{ |t| t.font_file == @unique_typography.font_file &&
                                                       t.font_size == @unique_typography.font_size &&
                                                       t.bpp == @unique_typography.bpp }
    typography_names = typographies_identical.map{ |t| t.name }.uniq

    # Find a typography with a fallback character
    typography_with_fallback_character = typographies_identical.find { |t| t.fallback_character }
    if typography_with_fallback_character
      # Now get the actual fallback character (or 'skip')
      typography_fallback_character = typography_with_fallback_character.fallback_character
      # Check to see if one of the other typographes has a different fallback character
      index = typographies_identical.find_index{ |t| t.fallback_character && t.fallback_character != typography_fallback_character }
      if index
        abort "The fallback character differs for typography \"#{typography_with_fallback_character.name}\" and typography \"#{typographies_identical[index].name}\""
      end
      # set all fallback characters to the same character
      typographies_identical.each { |t| t.fallback_character = typography_fallback_character }
    end

    # Find a typography with a ellipsis character
    typography_with_ellipsis_character = typographies_identical.find { |t| t.ellipsis_character }
    if typography_with_ellipsis_character
      # Now get the actual ellipsis character (or 'skip')
      typography_ellipsis_character = typography_with_ellipsis_character.ellipsis_character
      # Check to see if one of the other typographes has a different ellipsis character
      index = typographies_identical.find_index{ |t| t.ellipsis_character && t.ellipsis_character != typography_ellipsis_character }
      if index
        abort "The ellipsis character differs for typography \"#{typography_with_ellipsis_character.name}\" and typography \"#{typographies_identical[index].name}\""
      end
      # set all ellipsis characters to the same character
      typographies_identical.each { |t| t.ellipsis_character = typography_ellipsis_character }
    end

    all_translations = typography_names.map{ |typography_name| @text_entries.collect{ |entry| entry.translations_with_typography(typography_name) }.flatten }.flatten

    unicodes = all_translations.map(&:unicodes).flatten.uniq.sort

    typographies_identical.each do |t|
      fbc = t.fallback_character
      fbcUnicode = 0
      if fbc
        if fbc.downcase == 'skip'
          fbcUnicode = 0xFEFF
        elsif fbc.length == 1
          fbcUnicode = fbc[0].ord
        else
          begin
            fbcUnicode = Integer(fbc.gsub(/\.0*$/,''))
          rescue
            fail "Please only specify one character or ('skip') as Fallback Character, typography \"#{typography_with_fallback_character.name}\" has Fallback Character \"#{typography_with_fallback_character.fallback_character}\""
          end
        end
        unicodes += [ fbcUnicode ]
      end
      t.fallback_character = fbcUnicode

      tec = t.ellipsis_character
      tecUnicode = 0
      if tec
        if tec.length == 1
          tecUnicode = tec[0].ord
        else
          begin
            tecUnicode = Integer(tec.gsub(/\.0*$/,''))
          rescue
            fail "Please only specify one character as Ellipsis Character for typography \"#{typography_with_fallback_character.name}\""
          end
        end
        unicodes += [ tecUnicode ]
      end
      t.ellipsis_character = tecUnicode
    end
    typographies_identical.each{ |t|
      if t.wildcard_characters
        t.wildcard_characters.to_s.split('').each { |c|
          unicodes += [ c[0].ord ]
        }
      end
      if t.wildcard_ranges
        unicodes += decode_ranges(t.wildcard_ranges)
      end
    }
    
    unicodes = convert_to_contextual_forms(unicodes)

    unicodes = mirror_brackes(unicodes)

    unicodes = unicodes.uniq.sort

    check_for_rtl(unicodes)

    FileIO.write_file(File.join(@output_directory, "UnicodeList#{@unique_typography.cpp_name}_#{@unique_typography.font_size}_#{@unique_typography.bpp}.txt"), unicodes.join(LINE_ENDINGS) )
  end
end
