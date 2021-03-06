/*************************************************************************
 * libmatrix
 *************************************************************************
 * @file    CMatrix.cpp
 * @date    21.01.15
 * @author  Hlieb Romanov <rgewebppc@gmail.com>
 * @brief   The CMatrix class definition
 ************************************************************************/
#include "CMatrix.hpp"
#include "CMatrixException.hpp"

using namespace matrix;

CMatrix::CMatrix( void )
    : mMatrix( 0 )
{
}

CMatrix::CMatrix( size_t rows, size_t cols )
{
    mMatrix = gsl_matrix_calloc( rows, cols );
    if ( !mMatrix )
    {
        throw CMatrixException( "Can't allocate memory for matrix" );
    }
}

CMatrix::CMatrix( const CMatrix& other )
{
    mMatrix = matrixAlloc( other.rows(), other.cols() );
    copyRawMatrix( mMatrix, other.mMatrix );
}

CMatrix& CMatrix::operator=( const CMatrix& other )
{
    if ( this != &other )
    {
        if ( rows() != other.rows() || cols() != other.cols() )
        {
            gsl_matrix* newMatrix = matrixAlloc( other.rows(), other.cols() );
            copyRawMatrix( newMatrix, other.mMatrix );
            gsl_matrix_free( mMatrix );
            mMatrix = newMatrix;
        }
        else
        {
            copyRawMatrix( mMatrix, other.mMatrix );
        }
    }
    return *this;
}

CMatrix::~CMatrix( void )
{
    if ( isValid() )
    {
        gsl_matrix_free( mMatrix );
    }
}

size_t CMatrix::rows( void ) const
{
    validateMatrix();
    return mMatrix->size1;
}

size_t CMatrix::cols() const
{
    validateMatrix();
    return mMatrix->size2;
}

double CMatrix::get( size_t rowIndex, size_t columnIndex ) const
{
    validateMatrix();
    return gsl_matrix_get( mMatrix, rowIndex, columnIndex );
}

void CMatrix::set( size_t rowIndex, size_t columnIndex, double value )
{
    validateMatrix();
    gsl_matrix_set( mMatrix, rowIndex, columnIndex, value );
}

const double& CMatrix::operator()( size_t rowIndex, size_t columnIndex ) const
{
    validateMatrix();
    return *gsl_matrix_const_ptr( mMatrix, rowIndex, columnIndex );
}

double& CMatrix::operator()( size_t rowIndex, size_t columnIndex )
{
    validateMatrix();
    return *gsl_matrix_ptr( mMatrix, rowIndex, columnIndex );
}

void CMatrix::swap( CMatrix& other )
{
    using std::swap;

    swap( mMatrix, other.mMatrix );
}

void CMatrix::copyRawMatrix( gsl_matrix* destination, gsl_matrix* source )
{
    validateMatrix();
    int result = gsl_matrix_memcpy( destination, source );
    if ( result != GSL_SUCCESS )
    {
        throw CMatrixException( "Can't copy matrix" );
    }
}

gsl_matrix* CMatrix::matrixAlloc( size_t rows, size_t cols )
{
    gsl_matrix* matrix = gsl_matrix_alloc( rows, cols );

    if ( !matrix )
    {
        throw CMatrixException( "Can't allocate memory for matrix" );
    }

    return matrix;
}

inline void CMatrix::validateMatrix( void ) const
{
    if ( !isValid() )
    {
        throw CMatrixException( "Invalid matrix. Operation not performed." );
    }
}

