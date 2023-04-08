#include "query/with_ref.h"

namespace qps {

// Constructor with assignment to Attribute Type
WithRef::WithRef(WithRefType ref_) : ref{ ref_ }, attrType{ AttrType::INTEGER } {
	if (std::holds_alternative<QuotedIdentifier>(ref_)) {
		attrType = AttrType::NAME;
	}
	else if (std::holds_alternative<int>(ref_)) {
		attrType = AttrType::INTEGER;
	}
	else if (std::holds_alternative<AttrRef>(ref_)) {
		AttrRef attrRef = std::get<AttrRef>(ref_);
		attrType = getAttrType(attrRef.attrName);
	}
}

}  // namespace qps
