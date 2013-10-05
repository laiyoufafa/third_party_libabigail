// -*- Mode: C++ -*-
//
// Copyright (C) 2013 Red Hat, Inc.
//
// This file is part of the GNU Application Binary Interface Generic
// Analysis and Instrumentation Library (libabigail).  This library is
// free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the
// Free Software Foundation; either version 3, or (at your option) any
// later version.

// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this program; see the file COPYING-LGPLV3.  If
// not, see <http://www.gnu.org/licenses/>.

#include <cstring>
#include "abg-diff-utils.h"

/// @file
///
/// This file defines the declarations found in abg-diff-utils.h
namespace abigail
{

namespace diff_utils
{
/// @return true iff the end of a furthest forward d_path overlaps the
/// end of a furtherst reverse d_path on the same diagonal.  This is
/// defined by the lemma 3 of the paper.
///
/// @param forward_d_path_end
bool
ends_of_furthest_d_paths_overlap(point& forward_d_path_end,
				  point& reverse_d_path_end)
{
  return ((forward_d_path_end.x() - forward_d_path_end.y())
	  == (reverse_d_path_end.x() - reverse_d_path_end.y())
	  && forward_d_path_end.x() >= reverse_d_path_end.x());
}
//// Test if a point is within the boundaries of the edit graph.
///
/// @param p the point to test.
///
/// @param a_size the size of abscissas.  That is, the size of the
/// first input to consider.
///
/// @param b_size the of ordinates.  That is, the size of the second
/// input to consider.
///
/// @return true iff a point is within the boundaries of the edit
/// graph.
bool
point_is_valid_in_graph(point& p,
			unsigned a_size,
			unsigned b_size)
{
  return (p.x() > -1
	  && p.x() < (int) a_size
	  && p.y() > -1
	  && p.y() < (int) b_size);
}

/// Returns the middle snake of two strings, as well as the length of
/// their shortest editing script.
///
/// @param str1 the first string to consider.
///
/// @param str2 the second string to consider.
///
/// @param snake_begin the point representing the beginning
bool
compute_middle_snake(const char* str1, const char* str2,
		     point& snake_begin, point& snake_end,
		     int& ses_len)
{
  bool has_snake = false;
  int str1_size = strlen(str1), str2_size = strlen(str2);

  if (compute_middle_snake(str1, str1 + str1_size,
			   str2 , str2 + str2_size,
			   snake_begin, snake_end,
			   ses_len))
    has_snake = true;

  return has_snake;
}

/// Compute the length of the shortest edit script for two strings.
/// This is done using the "Greedy LCS/SES" of figure 2 in the paper.
/// It can walk the edit graph either foward (when reverse is false)
/// or backward starting from the end (when reverse is true).
///
/// @param str1 the first string to consider.
///
/// @param str2 the second string to consider.
///
/// @param reverse when true, walk the edit graph backward, starting
/// from the point (M,N) (with M and N being the length of str1 and
/// str2).  When false, walk the edit graph forward, starting from the
/// point (0,0).
///
/// @return the length of the shortest edit script.
int
ses_len(const char* str1,
	const char* str2,
	bool reverse)
{
  int str1_size = strlen(str1), str2_size = strlen(str2);

  d_path_vec v(str1_size, str2_size);
  return ses_len(str1, str1 + str1_size,
		 str2, str2 + str2_size,
		 v, reverse);
}

/// Compute the longest common subsequence of two strings and return
/// the length of the shortest edit script for transforming the first
/// string into the second.
///
/// @param str1 the first string to consider.
///
/// @param str2 the second string to consider.
///
/// @param ses_len the length of the shortest edit script.  This is
/// set by the function iff it returns true.
///
/// @param the longest common subsequence of the two strings.  This is
/// set the function iff it returns true.
///
/// @return true if the function could compute an lcs, false
/// otherwise.
void
compute_lcs(const char* str1, const char* str2, int &ses_len, string& lcs)
{
  vector<point> result;
  edit_script ses;

  compute_diff(str1, str1 + strlen(str1),
	       str2, str2 + strlen(str2),
	       result, ses);

  ses_len = ses.length();

  for (unsigned i = 0; i < result.size(); ++i)
    {
      int x = result[i].x(), y = result[i].y();
      assert(str1[x] == str2[y]);
      lcs.push_back(str1[x]);
    }
}

/// Compute the shortest edit script for transforming a string into
/// another.
///
/// @param str1 the first string to consider.
///
/// @param str2 the second string to consider.
///
/// @param ses the resulting edit script.
///
/// @pram ses_len the length of the edit script.
void
compute_ses(const char* str1, const char* str2, edit_script& ses)
{
  vector<point> lcs;
  compute_diff(str1, str1 + strlen(str1),
	       str2, str2 + strlen(str2),
	       lcs, ses);
}

}//end namespace diff_utils

}//end namespace abigail
