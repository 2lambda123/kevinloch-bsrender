//
// Billion Star 3D Rendering Engine
// Kevin M. Loch
//
// 3D rendering engine for the ESA Gaia EDR3 star dataset

/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2021, Kevin Loch
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "bsrender.h" // needs to be first to get GNU_SOURCE define for strcasestr
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "usage.h"

void initConfig(bsr_config_t *bsr_config) {
  strcpy(bsr_config->config_file_name, "bsrender.cfg");
  strcpy(bsr_config->data_file_directory, "galaxydata");
  strcpy(bsr_config->output_file_name, "galaxy.png");
  bsr_config->print_status=1;
  bsr_config->num_threads=16;
  bsr_config->per_thread_buffer=1000;
  bsr_config->per_thread_buffer_Airy=100000;
  bsr_config->cgi_mode=0;
  bsr_config->cgi_max_res_x=999999;
  bsr_config->cgi_max_res_y=999999;
  bsr_config->cgi_Gaia_min_parallax_quality=0;
  bsr_config->cgi_allow_Airy_disk=1;
  bsr_config->cgi_min_Airy_disk_first_null=0.3;
  bsr_config->cgi_max_Airy_disk_max_extent=1000;
  bsr_config->cgi_max_Airy_disk_min_extent=3;
  bsr_config->cgi_allow_anti_alias=1;
  bsr_config->Gaia_db_enable=1;
  bsr_config->Gaia_min_parallax_quality=0;
  bsr_config->external_db_enable=1;
  bsr_config->render_distance_min=0.0;
  bsr_config->render_distance_max=1.0E99;
  bsr_config->render_distance_selector=0;
  bsr_config->star_color_min=0.0;
  bsr_config->star_color_max=1.0E99;
  bsr_config->camera_res_x=4000;
  bsr_config->camera_res_y=2000;
  bsr_config->camera_fov=360.0;
  bsr_config->camera_pixel_limit_mag=8.0;
  bsr_config->camera_pixel_limit_mode=0;
  bsr_config->camera_wb_enable=1;
  bsr_config->camera_wb_temp=4300.0;
  bsr_config->camera_color_saturation=1.0;
  bsr_config->camera_gamma=1.0;
  bsr_config->camera_projection=0;
  bsr_config->spherical_orientation=0;
  bsr_config->Mollewide_iterations=5;
  bsr_config->red_filter_long_limit=705.0;
  bsr_config->red_filter_short_limit=550.0;
  bsr_config->green_filter_long_limit=600.0;
  bsr_config->green_filter_short_limit=445.0;
  bsr_config->blue_filter_long_limit=465.0;
  bsr_config->blue_filter_short_limit=395.0;
  bsr_config->Airy_disk_enable=0;
  bsr_config->Airy_disk_first_null=0.75;
  bsr_config->Airy_disk_max_extent=100;
  bsr_config->Airy_disk_min_extent=1;
  bsr_config->Airy_disk_obstruction=0.0;
  bsr_config->anti_alias_enable=0;
  bsr_config->anti_alias_radius=1.0;
  bsr_config->skyglow_enable=0;
  bsr_config->skyglow_temp=4500.0;
  bsr_config->skyglow_per_pixel_mag=14.0;
  bsr_config->Gaussian_blur_radius=0.0;
  bsr_config->output_scaling_factor=1.0;
  bsr_config->draw_crosshairs=0;
  bsr_config->draw_grid_lines=0;
  bsr_config->sRGB_gamma=1;
  bsr_config->bits_per_color=8;
  bsr_config->camera_icrs_x=0.0;
  bsr_config->camera_icrs_y=0.0;
  bsr_config->camera_icrs_z=0.0;
  bsr_config->camera_icrs_ra=0.0;
  bsr_config->camera_icrs_dec=0.0;
  bsr_config->camera_icrs_r=0.0;
  bsr_config->target_icrs_x=0.0;
  bsr_config->target_icrs_y=0.0;
  bsr_config->target_icrs_z=0.0;
  bsr_config->target_icrs_ra=266.4168371;
  bsr_config->target_icrs_dec=-29.0078106;
  bsr_config->target_icrs_r=8178.0;
  bsr_config->camera_rotation=-58.6;
  bsr_config->camera_pan=0.0;
  bsr_config->camera_tilt=0.0;
}

void cleanupValueStr(char *value) {
  int i;
  int j;
  int start;
  int end;
  char tmpvalue[256];
  size_t value_length;

  //
  // safety bounds check
  //
  value_length=strlen(value);
  if (value_length > 254) {
    value_length=254;
  }
  
  //
  // find beginning of value, ignoring leading spaces or quotes
  //
  start=-1;
  for (i=0; ((start == -1) && (i < value_length)); i++) {
    if ((value[i] != 32) && (value[i] != 34) && (value[i] != 39)) { 
      start=i;
    }
  }

  //
  // find end of value, ignoring trailing spaces or quotes
  //
  end=-1;
  for (i=(value_length-1); ((end == -1) && (i >= 0)); i--) {
    if ((value[i] != 32) && (value[i] != 34) && (value[i] != 39)) {
      end=i;
    }
  }

  //
  // trim before and after value
  //
  j=0;
  for (i=start; i <= end; i++) {
    tmpvalue[j]=value[i];
    j++;
  }
  tmpvalue[j]=0;

  strcpy(value, tmpvalue);
}

void checkOptionBool(int *config_int, char *option, char *value, char *matchstr) {
  if ((strcasestr(option, matchstr) == option) && (strlen(option) == strlen(matchstr))) {
    if (strcasestr(value, "yes") != NULL) {
      *config_int=1;
    } else {
      *config_int=0;
    }
  }
}

void checkOptionInt(int *config_int, char *option, char *value, char *matchstr) {
  if ((strcasestr(option, matchstr) == option) && (strlen(option) == strlen(matchstr))) {
    *config_int=strtol(value, NULL, 10);
  }
}

void checkOptionDouble(double *config_double, char *option, char *value, char *matchstr) {
  if ((strcasestr(option, matchstr) == option) && (strlen(option) == strlen(matchstr))) {
    *config_double=strtod(value, NULL);
  }
}

void checkOptionStr(char *config_str,  char *option, char *value, char *matchstr) {
  if ((strcasestr(option, matchstr) == option) && (strlen(option) == strlen(matchstr))) {
    strcpy(config_str, value);
  }
}

void setOptionValue(bsr_config_t *bsr_config, char *option, char *value, int from_cgi) {
  //
  // privileged values that can be set from config file or command line only (not CGI QUERY_STRING)
  // note: config file name is additionally privileged in that it can only be set from command line
  // so it is not even processed in this function.
  //
  if (from_cgi == 0) {
    checkOptionStr(bsr_config->data_file_directory, option, value, "data_file_directory");
    checkOptionStr(bsr_config->output_file_name, option, value, "output_file_name");
    checkOptionBool(&bsr_config->print_status, option, value, "print_status");
    checkOptionInt(&bsr_config->num_threads, option, value, "num_threads");
    checkOptionInt(&bsr_config->per_thread_buffer, option, value, "per_thread_buffer");
    checkOptionInt(&bsr_config->per_thread_buffer_Airy, option, value, "per_thread_buffer_Airy");
    checkOptionBool(&bsr_config->cgi_mode, option, value, "cgi_mode");
    checkOptionInt(&bsr_config->cgi_max_res_x, option, value, "cgi_max_res_x");
    checkOptionInt(&bsr_config->cgi_max_res_y, option, value, "cgi_max_res_y");
    checkOptionInt(&bsr_config->cgi_Gaia_min_parallax_quality, option, value, "cgi_Gaia_min_parallax_quality");
    checkOptionBool(&bsr_config->cgi_allow_Airy_disk, option, value, "cgi_allow_Airy_disk");
    checkOptionDouble(&bsr_config->cgi_min_Airy_disk_first_null, option, value, "cgi_min_Airy_disk_first_null");
    checkOptionInt(&bsr_config->cgi_max_Airy_disk_max_extent, option, value, "cgi_max_Airy_disk_max_extent");
    checkOptionInt(&bsr_config->cgi_max_Airy_disk_min_extent, option, value, "cgi_max_Airy_disk_min_extent");
    checkOptionBool(&bsr_config->cgi_allow_anti_alias, option, value, "cgi_allow_anti_alias");
  }

  //
  // values that can be set from config file, command line, or CGI QUERY_STRING
  //
  checkOptionBool(&bsr_config->Gaia_db_enable, option, value, "Gaia_db_enable");
  checkOptionInt(&bsr_config->Gaia_min_parallax_quality, option, value, "Gaia_min_parallax_quality");
  checkOptionBool(&bsr_config->external_db_enable, option, value, "external_db_enable");
  checkOptionDouble(&bsr_config->render_distance_min, option, value, "render_distance_min");
  checkOptionDouble(&bsr_config->render_distance_max, option, value, "render_distance_max");
  checkOptionInt(&bsr_config->render_distance_selector, option, value, "render_distance_selector");
  checkOptionDouble(&bsr_config->star_color_min, option, value, "star_color_min");
  checkOptionDouble(&bsr_config->star_color_max, option, value, "star_color_max");
  checkOptionInt(&bsr_config->camera_res_x, option, value, "camera_res_x");
  checkOptionInt(&bsr_config->camera_res_y, option, value, "camera_res_y");
  checkOptionDouble(&bsr_config->camera_fov, option, value, "camera_fov");
  checkOptionDouble(&bsr_config->camera_pixel_limit_mag, option, value, "camera_pixel_limit_mag");
  checkOptionInt(&bsr_config->camera_pixel_limit_mode, option, value, "camera_pixel_limit_mode");
  checkOptionBool(&bsr_config->camera_wb_enable, option, value, "camera_wb_enable");
  checkOptionDouble(&bsr_config->camera_wb_temp, option, value, "camera_wb_temp");
  checkOptionDouble(&bsr_config->camera_color_saturation, option, value, "camera_color_saturation");
  checkOptionDouble(&bsr_config->camera_gamma, option, value, "camera_gamma");
  checkOptionInt(&bsr_config->camera_projection, option, value, "camera_projection");
  checkOptionInt(&bsr_config->spherical_orientation, option, value, "spherical_orientation");
  checkOptionInt(&bsr_config->Mollewide_iterations, option, value, "Mollewide_iterations");
  checkOptionDouble(&bsr_config->red_filter_long_limit, option, value, "red_filter_long_limit");
  checkOptionDouble(&bsr_config->red_filter_short_limit, option, value, "red_filter_short_limit");
  checkOptionDouble(&bsr_config->green_filter_long_limit, option, value, "green_filter_long_limit");
  checkOptionDouble(&bsr_config->green_filter_short_limit, option, value, "green_filter_short_limit");
  checkOptionDouble(&bsr_config->blue_filter_long_limit, option, value, "blue_filter_long_limit");
  checkOptionDouble(&bsr_config->blue_filter_short_limit, option, value, "blue_filter_short_limit");
  checkOptionBool(&bsr_config->Airy_disk_enable, option, value, "Airy_disk_enable");
  checkOptionDouble(&bsr_config->Airy_disk_first_null, option, value, "Airy_disk_first_null");
  checkOptionInt(&bsr_config->Airy_disk_max_extent, option, value, "Airy_disk_max_extent");
  checkOptionInt(&bsr_config->Airy_disk_min_extent, option, value, "Airy_disk_min_extent");
  checkOptionDouble(&bsr_config->Airy_disk_obstruction, option, value, "Airy_disk_obstruction");
  checkOptionBool(&bsr_config->anti_alias_enable, option, value, "anti_alias_enable");
  checkOptionDouble(&bsr_config->anti_alias_radius, option, value, "anti_alias_radius");
  checkOptionBool(&bsr_config->skyglow_enable, option, value, "skyglow_enable");
  checkOptionDouble(&bsr_config->skyglow_temp, option, value, "skyglow_temp");
  checkOptionDouble(&bsr_config->skyglow_per_pixel_mag, option, value, "skyglow_per_pixel_mag");
  checkOptionDouble(&bsr_config->Gaussian_blur_radius, option, value, "Gaussian_blur_radius");
  checkOptionDouble(&bsr_config->output_scaling_factor, option, value, "output_scaling_factor");
  checkOptionBool(&bsr_config->draw_crosshairs, option, value, "draw_crosshairs");
  checkOptionBool(&bsr_config->draw_grid_lines, option, value, "draw_grid_lines");
  checkOptionBool(&bsr_config->sRGB_gamma, option, value, "sRGB_gamma");
  checkOptionInt(&bsr_config->bits_per_color, option, value, "bits_per_color");
  checkOptionDouble(&bsr_config->camera_icrs_x, option, value, "camera_icrs_x");
  checkOptionDouble(&bsr_config->camera_icrs_y, option, value, "camera_icrs_y");
  checkOptionDouble(&bsr_config->camera_icrs_z, option, value, "camera_icrs_z");
  checkOptionDouble(&bsr_config->camera_icrs_ra, option, value, "camera_icrs_ra");
  checkOptionDouble(&bsr_config->camera_icrs_dec, option, value, "camera_icrs_dec");
  checkOptionDouble(&bsr_config->camera_icrs_r, option, value, "camera_icrs_r");
  checkOptionDouble(&bsr_config->target_icrs_x, option, value, "target_icrs_x");
  checkOptionDouble(&bsr_config->target_icrs_y, option, value, "target_icrs_y");
  checkOptionDouble(&bsr_config->target_icrs_z, option, value, "target_icrs_z");
  checkOptionDouble(&bsr_config->target_icrs_ra, option, value, "target_icrs_ra");
  checkOptionDouble(&bsr_config->target_icrs_dec, option, value, "target_icrs_dec");
  checkOptionDouble(&bsr_config->target_icrs_r, option, value, "target_icrs_r");
  checkOptionDouble(&bsr_config->camera_rotation, option, value, "camera_rotation");
  checkOptionDouble(&bsr_config->camera_pan, option, value, "camera_pan");
  checkOptionDouble(&bsr_config->camera_tilt, option, value, "camera_tilt");
}

int processConfigSegment(bsr_config_t *bsr_config, char *segment, int from_cgi) {
  int segment_length;
  char *symbol_p;
  char option[256];
  size_t option_length;
  char value[256];
  size_t value_length;

  //
  // special handling for --help
  //
  if ((strcasestr(segment, "help") == segment) && (strlen(segment) == strlen("help"))) {
    printUsage();
    exit(0);
  }

  //
  // search for option/value delimiter and split option and value strings
  //
  segment_length=strlen(segment);
  symbol_p=strchr(segment, '=');
  if ((segment_length > 0) && (symbol_p != NULL)) {
    option_length=(symbol_p - segment);
    value_length=(segment_length - option_length);

    //
    // enforce range restrictions on option and value
    //
    if ((option_length < 254) && (value_length < 254)) {
      strncpy(option, segment, option_length);
      option[option_length]=0;
      strncpy(value, (symbol_p+=1), value_length);
      value[value_length]=0;

      //
      // remove non-alphanumeric characters before and after value
      //
      cleanupValueStr(value);

      //
      // send to option value processing fucntion
      //
      setOptionValue(bsr_config, option, value, 0); // 0 == not from cgi

    } // end option_length and value_length checks
  } // end symbol_p check

  return(0);
}


int loadConfigFromFile(bsr_config_t *bsr_config) {
  int from_cgi;
  FILE *config_file;
  char *input_line_p;
  char *symbol_p;
  size_t input_line_length;
  char input_line[256];
  char segment[256];
  size_t segment_length;
  char *query_string;
  
  //
  // options from config file are not from remote CGI users
  //
  from_cgi=0;

  //
  // print status udpate if not in CGI mode. We use the existence of QUERY_STRING to guess CGI mode at this point
  //
  query_string=NULL;
  query_string=getenv("QUERY_STRING");
  if ((query_string == NULL) && (bsr_config->print_status == 1)) {
    printf("bsrender version %s\n", BSR_VERSION);
    printf("Loading configuration file %s\n", bsr_config->config_file_name);
    fflush(stdout);
  }

  //
  // attempt to open config file
  //
  config_file=fopen(bsr_config->config_file_name, "r");
  if ((config_file == NULL) && (query_string == NULL)) {
    printf("Warning: could not open %s\n", bsr_config->config_file_name);
    fflush(stdout);
    return(0);
  }

  //
  // read and process each line of config file
  //
  input_line_p=fgets(input_line, 256, config_file);
  while(input_line_p != NULL) {
    //
    // search for comment symbol and remove any comments, otherwise just remove newline
    //
    input_line_length=strlen(input_line);
    symbol_p=strchr(input_line, '#');
    if (symbol_p != NULL) {
      segment_length=(symbol_p - input_line);
    } else {
      segment_length=input_line_length-1;
    } 
    strncpy(segment, input_line, segment_length);
    segment[segment_length]=0;

    //
    // process config segment
    //
    from_cgi=0;
    processConfigSegment(bsr_config, segment, from_cgi);

    //
    // load next line from config file
    //
    input_line_p=fgets(input_line, 256, config_file);
  } // end while input_line_raw

  return(0);
}

int loadConfigFromQueryString(bsr_config_t *bsr_config, char *query_string) {
  int done;
  int from_cgi;
  char *query_p;
  char segment[2048];
  int segment_length;
  char *symbol_p;

  //
  // any config segments in this function are from cgi. Set to 1 to enforce privileged config options
  //
  from_cgi=1;

  //
  // load first segment from query_string
  //
  done=0;
  query_p=query_string;
  if (query_p[0] == 0) {
    done=1;
  } else {
    symbol_p=strchr(query_p, '&');
    if (symbol_p != NULL) {
      segment_length=symbol_p - query_p;
    } else {
      segment_length=strlen(query_p);
    }
  }

  //
  // loop for processing segments from query string
  //
  while (done == 0) {
    //
    // process config segment
    //
    strncpy(segment, query_p, segment_length);
    segment[segment_length]=0;
    processConfigSegment(bsr_config, segment, from_cgi);

    //
    // load next segment from query string
    //
    query_p+=segment_length;
    if (query_p[0] == 0) {
      done=1;
    } else {
      query_p++;
      symbol_p=strchr(query_p, '&');
      if (symbol_p != NULL) {
        segment_length=symbol_p - query_p;
      } else {
        segment_length=strlen(query_p);
      }
    }
  } // end while not done

  return(0);
}

int processCmdArgs(bsr_config_t *bsr_config, int argc, char **argv) {
  int i;
  int from_cgi;
  char *option_start;

  if (argc == 1) {
    return(0);
  } else {
    for (i=1; i <= (argc - 1); i++) {
      if (argv[i][1] == 'c') {
        // configuration file name
        if (argv[i][2] != 0) {
          // option concatenated onto switch
          option_start=argv[i];
          strcpy(bsr_config->config_file_name, (option_start + (size_t)2));
        } else if ((argc >= (i + 1)) && (argv[i + 1][0] != '-')) {
          // option is probably next argv
          option_start=argv[i + 1];
          strcpy(bsr_config->config_file_name, option_start);
        } // end if no space

      } else if (argv[i][1] == 'd') {
        // data files directory
        if (argv[i][2] != 0) {
          // option concatenated onto switch
          option_start=argv[i];
          strcpy(bsr_config->data_file_directory, (option_start + (size_t)2));
        } else if ((argc >= (i + 1)) && (argv[i + 1][0] != '-')) {
          // option is probably next argv
          option_start=argv[i + 1];
          strcpy(bsr_config->data_file_directory, option_start);
        } // end if no space
      } else if (argv[i][1] == 'h') {
        // print help
        printUsage();
        exit(0);
      } else if (argv[i][1] == 'o') {
        // output filename
        if (argv[i][2] != 0) {
          // option concatenated onto switch
          option_start=argv[i];
          strcpy(bsr_config->output_file_name, (option_start + (size_t)2));
        } else if ((argc >= (i + 1)) && (argv[i + 1][0] != '-')) {
          // option is probably next argv
          option_start=argv[i + 1];
          strcpy(bsr_config->output_file_name, option_start);
        } // end if no space
      } else if (argv[i][1] == 'q') {
        // quite mode - suppress non-error status messages
        bsr_config->print_status=0;
      } else if (argv[i][1] == '-') {
        // long config option
        from_cgi=0;
        processConfigSegment(bsr_config, (argv[i]+2), from_cgi);
      } // end which option
    } // end for argc
  } // end if any options
  return(0);
}
