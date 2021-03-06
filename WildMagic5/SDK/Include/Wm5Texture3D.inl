// Geometric Tools, LLC
// Copyright (c) 1998-2013
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.0 (2010/01/01)

//----------------------------------------------------------------------------
inline int Texture3D::GetWidth () const
{
    return GetDimension(0, 0);
}
//----------------------------------------------------------------------------
inline int Texture3D::GetHeight () const
{
    return GetDimension(1, 0);
}
//----------------------------------------------------------------------------
inline int Texture3D::GetThickness () const
{
    return GetDimension(2, 0);
}
//----------------------------------------------------------------------------
