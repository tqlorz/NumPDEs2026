/**
 * @file          ConnectedRegion.cpp
 * @author        zsh945
 * @date          2026-03-26
 */

#include "ConnectedRegion.hpp"

/// @brief Constructor for ConnectedRegion class
ConnectedRegion::ConnectedRegion() {
    _Image.init(0, 0);
    _LK.init(0);
    _TempImage.init(0, 0);
}

/// @brief Constructor for ConnectedRegion class
ConnectedRegion::ConnectedRegion(const Matrix_Int& Image) {
    _Image = Image;
    _LK.init(_Image.row() * _Image.col());
    for (int i = 0; i < _LK.size(); i++){ _LK(i) = i; }
    _TempImage.init(_Image.row(), _Image.col());
}

/// @brief Initialize the equivalence table for connected region labeling
int ConnectedRegion::InitEquivTable() {
	for (int i = 0; i < _Image.row(); i++)
		for (int j = 0; j < _Image.col(); j++)
			_TempImage(i, j) = 0;

	int LeftData, UpData;
	int Left, Up;
	int label = 1;
	int Lmax, Lmin;
	for (int i = 0; i < _Image.row(); i++) {
		for (int j = 0; j < _Image.col(); j++) {
			if (_Image(i, j) == 0) continue;

			LeftData = ((j - 1) >= 0) ? _Image(i, j - 1) : 0;
			UpData = ((i - 1) >= 0) ? _Image(i - 1, j) : 0;
			Left = ((j - 1) >= 0) ? _TempImage(i, j - 1) : 0;
			Up = ((i - 1) >= 0) ? _TempImage(i - 1, j) : 0;
			if (LeftData == BOUNDARY_POINT && UpData == BOUNDARY_POINT) {
				_TempImage(i, j) = label;
				label++;
			} else if (LeftData == INTERNAL_POINT && UpData == BOUNDARY_POINT) {
				_TempImage(i, j) = Left;
			} else if (LeftData == BOUNDARY_POINT && UpData == INTERNAL_POINT) {
				_TempImage(i, j) = Up;
			} else if (LeftData == INTERNAL_POINT && UpData == INTERNAL_POINT) {
				_TempImage(i, j) = Left;
				if (Left != Up) {
					Lmax = max(Left, Up);
					Lmin = min(Left, Up);
					while (_LK(Lmax) != Lmax) { Lmax = _LK(Lmax); }
					while (_LK(Lmin) != Lmin) { Lmin = _LK(Lmin); }
					if (Lmax > Lmin) {
						_LK(Lmax) = Lmin;
					} else {
						_LK(Lmin) = Lmax;
					}
				}
			}
		}
	}
    return label - 1;
}

/// @brief Sort the equivalence table for connected region labeling
void ConnectedRegion::SortEquivTable(const int iMax) {
	int temp;
	for (int i = 1; i <= iMax; i++) {
		if (_LK(i) != _LK(_LK(i))) {
			temp = _LK(i);
			while (_LK(temp) != temp) {
				temp = _LK(temp);
			}
			_LK(i) = temp;
		}
	}
}

/// @brief Relabel the equivalence table for connected region labeling
void ConnectedRegion::RelabelEquivTable(const int iMax) {
	_NumOfConnectedRegion = 1;
	for (int i = 1; i <= iMax; i++) {
		if (i == _LK(i)) {
			_LK(i) = _NumOfConnectedRegion++;
		} else {
			_LK(i) = _LK(_LK(i));
		}
	}
	_NumOfConnectedRegion--;
}

/// @brief Relabel the image based on the equivalence table for connected region labeling
void ConnectedRegion::RelabelImage() {
	for (int i = 0; i < _Image.row(); i++) {
		for (int j = 0; j < _Image.col(); j++) {
			_Image(i, j) = _LK(_TempImage(i, j));
		}
	}
}

void ConnectedRegion::GenerateConnectedImage() {
    int iMax;
    iMax = InitEquivTable();
    SortEquivTable(iMax);
    RelabelEquivTable(iMax);
    RelabelImage();
}

/// @brief Check whether the calculation region is connected
/// @return true if the region is connected, false otherwise
bool ConnectedRegion::IsConnected() const {
    return _NumOfConnectedRegion == 1;
}