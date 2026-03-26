/**
 * @file          ConnectedRegion.hpp
 * @author        zsh945
 * @date          2026-03-26
 * @brief         This file defines the ConnectedRegion class, 
 *                which is responsible for checking whether 
 *                the calculation region is connected.
 */

#ifndef _CONNECTEDREGION_
#define _CONNECTEDREGION_

#include "Tensor.hpp"
#include "MacroDef.hpp"

/// @brief Class for connected region labeling
/// @details The input need to be a binary image, where the boundary points are 
///          represented by macro BOUNDARY_POINT and the internal points are represented by macro INTERNAL_POINT. 
class ConnectedRegion {
private:
    Matrix_Int _Image, _TempImage;
    Vector_Int _LK;
    int _NumOfConnectedRegion;
    int InitEquivTable();
    void SortEquivTable(const int iMax);
    void RelabelEquivTable(const int iMax);
    void RelabelImage();
public:
    ConnectedRegion();
    ConnectedRegion(const Matrix_Int& Image);
    Matrix_Int Image() const { return _Image; }
    int NumOfConnectedRegion() const { return _NumOfConnectedRegion; }
    void GenerateConnectedImage();
    bool IsConnected() const;
};

#endif