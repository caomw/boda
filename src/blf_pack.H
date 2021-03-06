#ifndef _BLF_PACK_H_
#define _BLF_PACK_H_

#include"conv_common.H"

namespace boda 
{

  struct pyra_pack_t : virtual public nesi // NESI(help="pyramid packing")
  {
    virtual cinfo_t const * get_cinfo( void ) const; // required declaration for NESI support
    uint32_t num_upsamp_octaves; //NESI(default=0,help="number of upsampled octaves")
    uint32_t interval; //NESI(default=7,help="steps per octave (factor of 2)")
    u32_pt_t bin_sz; //NESI(default="1200 1200", help="bin size for packing (as many bins needed will be used)")
    u32_pt_t in_sz; //NESI(default="0 0",help="input size to create pyramid for")
    uint32_t pack_pad_mode; //NESI(default=2,help="padding mode: "
    //" <default> 0=total isolation (support size)"
    //" 1=support_sz/2 img_edge and bin edge padding (yields full support_sz halo around each scale. pad_interp=0 rec.)"
    //" 2=eff_input_pad/2 img_edge padding and bin edge padding (yields eff_input_pad around each scale)")
    p_u32_pt_t force_img_edge_pad; //NESI(help="if specified, force image edge padding to this value.")
    uint32_t pad_interp; //NESI(default=0,help="<default>0=padding is global mean value"
    // " 1=interpolate from image edge to global mean value input")

    // outputs
    vect_u32_pt_t sizes;
    vect_u32_box_t pads;          
    vect_u32_pt_w_t placements;
    uint32_t num_bins;
    pyra_pack_t( void ) : num_bins(0) { }
    void do_place( conv_support_info_t const & csi );
  };

  typedef vector< pyra_pack_t > vect_pyra_pack_t; 
  typedef shared_ptr< pyra_pack_t > p_pyra_pack_t; 
  typedef vector< p_pyra_pack_t > vect_p_pyra_pack_t;


  struct pyra_params_t : virtual public nesi // NESI(help="pyramid packing params")
  {
    virtual cinfo_t const * get_cinfo( void ) const; // required declaration for NESI support
    u32_pt_t align; //NESI(default="16 16", help="pyra per-dim alignment")
    u32_pt_t min_pad; //NESI(default="165 165", help="pyra per-dim minimum padding")
    u32_pt_t eff_tot_pad; //NESI(default="64 64", help="pyra per-dim minimum padding")
  };

  struct pyra_pack_cli_t : virtual public nesi, public has_main_t, public pyra_pack_t, public pyra_params_t // NESI(
			   // help="pyramid packing interactive CLI",
			   // bases=["has_main_t", "pyra_pack_t", "pyra_params_t"], type_id="pyra_pack")
  {
    virtual cinfo_t const * get_cinfo( void ) const; // required declaration for NESI support

    filename_t out_fn; //NESI(default="%(boda_output_dir)/out.txt",help="text output filename")

    u32_pt_t align; //NESI(default="16 16", help="pyra per-dim alignment")
    u32_pt_t min_pad; //NESI(default="165 165", help="pyra per-dim minimum padding")
    u32_pt_t eff_tot_pad; //NESI(default="64 64", help="pyra per-dim minimum padding")

    virtual void main( nesi_init_arg_t * nia );
  };

  typedef vector< pyra_pack_cli_t > vect_pyra_pack_cli_t; 
  typedef shared_ptr< pyra_pack_cli_t > p_pyra_pack_cli_t; 
  typedef vector< p_pyra_pack_cli_t > vect_p_pyra_pack_cli_t;


  struct img_t;
  typedef vector< img_t > vect_img_t; 
  typedef shared_ptr< img_t > p_img_t; 
  typedef vector< p_img_t > vect_p_img_t;


  struct img_pyra_pack_t : virtual public nesi, public pyra_pack_t // NESI(help="image pyramid packing", bases=["pyra_pack_t"])
  {
    virtual cinfo_t const * get_cinfo( void ) const; // required declaration for NESI support
    // outputs
    vect_p_img_t pyra_imgs;
    vect_p_img_t bin_imgs;
    void scale_and_pack_img_into_bins( p_img_t img_in );
    void do_place_imgs( conv_support_info_t const & csi );
  };

  typedef vector< img_pyra_pack_t > vect_img_pyra_pack_t; 
  typedef shared_ptr< img_pyra_pack_t > p_img_pyra_pack_t; 
  typedef vector< p_img_pyra_pack_t > vect_p_img_pyra_pack_t;

  struct img_pyra_pack_cli_t : virtual public nesi, public img_pyra_pack_t, public pyra_params_t, public has_main_t // NESI(
			       // help="image pyramid packing",
			       // bases=["img_pyra_pack_t","pyra_params_t","has_main_t"], 
			       // type_id="img_pyra_pack")
  {
    virtual cinfo_t const * get_cinfo( void ) const; // required declaration for NESI support

    filename_t img_in_fn; //NESI(help="input image filename",req=1)
    uint32_t write_images; //NESI(default=0,help="if true, write out stiched images")
    filename_t img_out_fn; // NESI(default="%(boda_output_dir)/out_%%s.png", help="format for filenames of"
                           //   " output image bin files. %%s will replaced with the bin index.")

    virtual void main( nesi_init_arg_t * nia );
  };

  typedef vector< img_pyra_pack_cli_t > vect_img_pyra_pack_cli_t; 
  typedef shared_ptr< img_pyra_pack_cli_t > p_img_pyra_pack_cli_t; 
  typedef vector< p_img_pyra_pack_cli_t > vect_p_img_pyra_pack_cli_t;


}

#endif /* _BLF_PACK_H_ */
