//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#include "goc/math/pwl_function.h"

#include "goc/exception/exception_utils.h"
#include "goc/string/string_utils.h"
#include "goc/math/number_utils.h"

using namespace std;
using namespace nlohmann;

namespace goc
{
PWLFunction::PWLFunction()
{
	domain_ = image_ = {INFTY, -INFTY};
}

void PWLFunction::AddPiece(const LinearFunction& piece)
{
	// If this piece is a continuation of the last piece, then we need to merge them into one piece to have the
	// function normalized.
	bool is_continuation_of_last_piece = !pieces_.empty() && epsilon_equal(pieces_.back().slope, piece.slope) &&
		epsilon_equal(pieces_.back().intercept, piece.intercept) &&
		epsilon_equal(pieces_.back().domain.right, piece.domain.left);
	
	if (!is_continuation_of_last_piece)
	{
		// Add the new piece if it is not a continuation.
		pieces_.push_back(piece);
	}
	else
	{
		// Merge pieces to have the function normalized.
		pieces_.back().domain.right = piece.domain.right;
		pieces_.back().image.right = piece.image.right;
	}
	
	// Update domain and image.
	if (domain_.left == INFTY) domain_.left = piece.domain.left;
	domain_.right = piece.domain.right;
	image_.left = min(image_.left, piece.image.left);
	image_.right = max(image_.right, piece.image.right);
}

void PWLFunction::PopPiece()
{
	pieces_.pop_back();
	domain_ = Empty() ? Interval(INFTY, -INFTY) : Interval(domain_.left, pieces_.back().domain.right);
	UpdateImage();
}

bool PWLFunction::Empty() const
{
	return pieces_.empty();
}

int PWLFunction::PieceCount() const
{
	return pieces_.size();
}

const vector<LinearFunction>& PWLFunction::Pieces() const
{
	return pieces_;
}

const LinearFunction& PWLFunction::Piece(int i) const
{
	return pieces_[i];
}

const LinearFunction& PWLFunction::FirstPiece() const
{
	return pieces_.front();
}

const LinearFunction& PWLFunction::LastPiece() const
{
	return pieces_.back();
}

int PWLFunction::PieceIncluding(double x) const
{
	// if x is outside the Domain(), throw exception.
	if (epsilon_bigger(domain_.left, x) || epsilon_smaller(domain_.right, x))
	{
		fail("PWLFunction::Value(" + STR(x) +") failed, becuase domain is " + STR(Domain()));
		return -1;
	}
	
	// Look for a piece that include x in their domain.
	for (int i = pieces_.size()-1; i >= 0; --i)
		if (pieces_[i].domain.Includes(x))
			return i;
	
	// The function is not continuous and x is not in the domain of any piece.
	fail("PWLFunction::Value(" + STR(x) +") failed, becuase x is not inside the domain of its pieces.");
	return -1;
}

Interval PWLFunction::Domain() const
{
	return domain_;
}

Interval PWLFunction::Image() const
{
	return image_;
}

double PWLFunction::Value(double x) const
{
	// if x is outside the Domain(), throw exception.
	if (epsilon_bigger(domain_.left, x) || epsilon_smaller(domain_.right, x))
	{
		fail("PWLFunction::Value(" + STR(x) +") failed, becuase domain is " + STR(Domain()));
		return -1;
	}
	
	// Look for a piece that include x in their domain.
	for (int i = pieces_.size()-1; i >= 0; --i)
		if (pieces_[i].domain.Includes(x))
			return pieces_[i].Value(x);
	
	// The function is not continuous and x is not in the domain of any piece.
	fail("PWLFunction::Value(" + STR(x) +") failed, becuase x is not inside the domain of its pieces.");
	return -1;
}

double PWLFunction::operator()(double x) const
{
	return Value(x);
}

double PWLFunction::PreValue(double y) const
{
	if (epsilon_bigger(image_.left, y) || epsilon_smaller(image_.right, y))
	{
		fail("PWLFunction::PreValue(" + STR(y) +") failed, becuase image is " + STR(Image()));
		return -1;
	}
	
	// Look for a piece that include x in their domain.
	for (int i = pieces_.size()-1; i >= 0; --i)
		if (pieces_[i].image.Includes(y))
			return pieces_[i].PreValue(y);
	
	// The function is not continuous and x is not in the domain of any piece.
	fail("PWLFunction::PreValue(" + STR(y) +") failed, becuase y is not inside the domain of its pieces.");
	return -1;
}

void PWLFunction::Print(std::ostream& os) const
{
	os << "[";
	for (int i = 0; i < PieceCount(); ++i)
	{
		if (i > 0) os << ", ";
		os << Piece(i);
	}
	os << "]";
}

bool PWLFunction::operator==(const PWLFunction& f) const
{
	if (PieceCount() != f.PieceCount()) return false;
	for (int i = 0; i < PieceCount(); ++i) if (Piece(i) != f.Piece(i)) return false;
	return true;
}

bool PWLFunction::operator!=(const PWLFunction& f) const
{
	return !(*this == f);
}

void PWLFunction::UpdateImage()
{
	image_ = {INFTY, -INFTY};
	for (int i = 0; i < pieces_.size(); ++i)
	{
		image_.left = min(image_.left, pieces_[i].image.left);
		image_.right = max(image_.right, pieces_[i].image.right);
	}
}

void from_json(const json& j, PWLFunction& f)
{
	for (auto& p: j) f.AddPiece(p);
}

void to_json(json& j, const PWLFunction& f)
{
	j = vector<LinearFunction>();
	for (auto& p: f.Pieces()) j.push_back(p);
}
} // namespace goc