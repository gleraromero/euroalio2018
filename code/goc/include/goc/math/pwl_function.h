//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#ifndef GOC_MATH_PWL_FUNCTION_H
#define GOC_MATH_PWL_FUNCTION_H

#include <iostream>
#include <vector>

#include "goc/lib/json.hpp"
#include "goc/math/interval.h"
#include "goc/math/linear_function.h"
#include "goc/print/printable.h"

namespace goc
{
// This class represents a piecewise linear function. It has a sequence of linear functions with bounded domains.
// Invariant: the linear functions are non overlapping and are increasing in domain.
// Invariant: the function is stored normalized. A function is normalized iif no two consecutive pieces have the same
// 			  slope, intercept, and share the end and beginning of their domains.
// Example: [p1={(1,2),(2,3)},p2={(2,3),(3,4)}] is not normalized. [p1={(1,2),(3,4)}] is normalized.
class PWLFunction : public Printable
{
public:
	// Creates an empty piecewise linear function.
	PWLFunction();
	
	// Adds the piece at the end of the function.
	// Keeps the normalization invariant automatically.
	void AddPiece(const LinearFunction& piece);
	
	// Removes the last piece from the function.
	// Precondition: PieceCount() > 0.
	void PopPiece();
	
	// Returns: if the function has no pieces.
	bool Empty() const;
	
	// Returns: the numer of pieces of the function.
	int PieceCount() const;
	
	// Returns: a vector with the function pieces.
	const std::vector<LinearFunction>& Pieces() const;
	
	// Returns: the i-th piece of the function.
	// Precondition: i > PieceCount().
	const LinearFunction& Piece(int i) const;
	
	// Returns: the first piece of the function.
	// Precondition: !Empty().
	const LinearFunction& FirstPiece() const;
	
	// Returns: the last piece of the function.
	// Precondition: !Empty().
	const LinearFunction& LastPiece() const;
	
	// Returns: the last piece index that includes x in its domain.
	// Precondition: x \in dom(p) for any piece p.
	int PieceIncluding(double x) const;
	
	// Returns: the smallest interval [m, M] that includes all pieces domains.
	// Observation: if Empty() then returns [INFTY, -INFTY].
	Interval Domain() const;
	
	// Returns: the smallest interval [m, M] that includes all pieces images.
	// Observation: if Empty() then returns [INFTY, -INFTY].
	Interval Image() const;
	
	// Returns: the evaluation of the piece that includes x in its domain.
	// Exception: if no piece includes x in its domain, it throws an exception.
	double Value(double x) const;
	
	// Returns: the evaluation of the piece that includes x in its domain.
	// Exception: if no piece includes x in its domain, it throws an exception.
	double operator()(double x) const;
	
	// Returns: the last x such that f(x) = y. Notice that if the function is not bijective it may contain multiple
	// x such that f(x) = y.
	// Exception: if no f(x) = y, then it throws an exception.
	double PreValue(double y) const;
	
	// Prints the function.
	// Format: [p1, p2, ..., pn].
	void Print(std::ostream& os) const;
	
	// Returns: if all the pieces of both functions are the same.
	bool operator==(const PWLFunction& f) const;
	
	// Returns: if any piece of both functions is different.
	bool operator!=(const PWLFunction& f) const;
	
private:
	// Updates the image_ attribute to keep it updated after a Pop() operation.
	void UpdateImage();
	
	std::vector<LinearFunction> pieces_;
	Interval domain_, image_;
};

// JSON format: [p1, p2, ..., pn].
void from_json(const nlohmann::json& j, PWLFunction& f);

void to_json(nlohmann::json& j, const PWLFunction& f);
} // namespace goc

#endif //GOC_MATH_PWL_FUNCTION_H
