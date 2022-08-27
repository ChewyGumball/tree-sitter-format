#include <tree_sitter_format/traversers/IndentationTraverser.h>

#include <tree_sitter_format/Constants.h>

#include <cassert>

using namespace std::literals::string_view_literals;

namespace {

using namespace tree_sitter_format;

std::string_view ChildFieldName(TSNode node, uint32_t childIndex) {
    const char* name = ts_node_field_name_for_child(node, childIndex);
    if(name == nullptr) {
        return std::string_view();
    } else {
        return name;
    }
}

enum class ScopeChange { None, IncreaseBefore, DecreaseAfter, Both };

bool IsCompoundStatementLike(TSSymbol symbol) {
    return symbol == COMPOUND_STATEMENT ||
           symbol == DECLARATION_LIST ||
           symbol == FIELD_DECLARATION_LIST;
}
    
template<size_t COUNT>
ScopeChange NonCompoundBodyScopeChange(TSNode node, uint32_t childIndex, const std::string_view (&allowedFieldNames)[COUNT], Style::Indentation allowedIndentation) {
    std::string_view fieldName = ChildFieldName(node, childIndex);
    for(auto allowedName : allowedFieldNames) {
        if (fieldName == allowedName) {
            TSNode child = ts_node_child(node, childIndex);
            TSSymbol childSymbol = ts_node_symbol(child);

            // Compound Statements handle their own indentation so they can be handled uniformly
            // Declaration Lists are handled the same as Compound Statements, but the grammar
            // requires they be different node types.
            if (IsCompoundStatementLike(childSymbol)) {
                return ScopeChange::None;
            }
            
            if (allowedIndentation != Style::Indentation::None) {
                // Since this isn't a compound statement, its a single statement body
                // so we increase before, and decrease after
                return ScopeChange::Both;
            }
        }
    }

    return ScopeChange::None;
}

ScopeChange IfStatementScopeChange(TSNode node, uint32_t childIndex, const Style& style) {
    assert(ts_node_symbol(node) == IF_STATEMENT);
    return NonCompoundBodyScopeChange(node, childIndex, {"consequence"sv, "alternative"sv}, style.indentation.ifStatements);
}

ScopeChange WhileLoopScopeChange(TSNode node, uint32_t childIndex, const Style& style) {
    assert(ts_node_symbol(node) == WHILE_LOOP);
    return NonCompoundBodyScopeChange(node, childIndex, {"body"sv}, style.indentation.whileLoops);
}

ScopeChange DoWhileLoopScopeChange(TSNode node, uint32_t childIndex, const Style& style) {
    assert(ts_node_symbol(node) == DO_WHILE_LOOP);
    return NonCompoundBodyScopeChange(node, childIndex, {"body"sv}, style.indentation.whileLoops);
}

ScopeChange CompoundStatementScopeChange(TSNode node, uint32_t childIndex, const Style& style) {
    // Compound statements are '{', (statement), ... , '}'
    // There may be no (statement) nodes, ie an empty block

    uint32_t childCount = ts_node_child_count(node);
    if (childCount == 2) {
        return ScopeChange::None;
    }
    TSNode parent = ts_node_parent(node); 
    TSSymbol parentSymbol = ts_node_symbol(parent);
    Style::Indentation indentation = Style::Indentation::None;
    if (parentSymbol == IF_STATEMENT) {
        indentation = style.indentation.ifStatements;
    } else if (parentSymbol == FOR_LOOP || parentSymbol == FOR_RANGE_LOOP) {
        indentation = style.indentation.forLoops;
    } else if (parentSymbol == WHILE_LOOP) {
        indentation = style.indentation.whileLoops;
    } else if (parentSymbol == DO_WHILE_LOOP) {
        indentation = style.indentation.doWhileLoops;
    } else if (parentSymbol == FUNCTION_DEFINITION) {
        indentation = style.indentation.functionDefinitions;
    } else if (parentSymbol == SWITCH_STATEMENT) {
        indentation = style.indentation.switchStatements;
    }  else if (parentSymbol == NAMESPACE) {
        indentation = style.indentation.namespaces;
    }  else if (parentSymbol == STRUCT_DEFINITION) {
        indentation = style.indentation.structDefinitions;
    }  else {
        indentation = style.indentation.genericScope;
    }
    // TODO: add case blocks, need to check if a single case body statement,
    //  otherwise its just a generic scope.

    // We are the opening brace
    if (childIndex == 0) {
        switch(indentation) {
            case Style::Indentation::BracesIndented:
            case Style::Indentation::BothIndented:
                return ScopeChange::IncreaseBefore;
            case Style::Indentation::BodyIndented:
            case Style::Indentation::None:
            default:
                return ScopeChange::None;        
        }
    } 
    
    // We are the first statement after the opening brace
    if (childIndex == 1) {
        switch(indentation) {
            case Style::Indentation::BodyIndented:
            case Style::Indentation::BothIndented:
            // If this is the only non brace child, we increment before, and decrement after
            // otherwise, we increment before, and the decrement will happen for the child
            // that comes before the end brace 
                return childCount == 3 ? ScopeChange::Both : ScopeChange::IncreaseBefore;
            case Style::Indentation::BracesIndented:
            case Style::Indentation::None:
            default:
                return ScopeChange::None;        
        }
    } 
    
    // We are the last statement before the closing brace
    if (childIndex == childCount - 2) {
        switch(indentation) {
            case Style::Indentation::BodyIndented:
            case Style::Indentation::BothIndented:
            // If this is the only non brace child, we increment before, and decrement after
            // otherwise, we decrement after, and the increment happened for the child that
            // came after the start brace
                return childCount == 3 ? ScopeChange::Both : ScopeChange::DecreaseAfter;
            case Style::Indentation::BracesIndented:
            case Style::Indentation::None:
            default:
                return ScopeChange::None;        
        }
    } 
    
    // We are the closing brace
    if (childIndex == childCount - 1) {
        switch(indentation) {
            case Style::Indentation::BracesIndented:
            case Style::Indentation::BothIndented:
                return ScopeChange::DecreaseAfter;
            case Style::Indentation::BodyIndented:
            case Style::Indentation::None:
            default:
                return ScopeChange::None;        
        }
    }

    // This is just a statement in the middle of a block, nothing special happens with
    // indentation here
    return ScopeChange::None;
}
    
ScopeChange ScopeChangeForChild(TSNode node, uint32_t childIndex, const Style& style) {
    TSSymbol symbol = ts_node_symbol(node);

    // These handle indentation for bodies that have a single statement in them.
    // Bodies with multiple statements are handled as compound statements (and similar brace enclosed nodes).
    if (symbol == IF_STATEMENT) {
        return IfStatementScopeChange(node, childIndex, style);
    } else if (symbol == WHILE_LOOP) {
        return WhileLoopScopeChange(node, childIndex, style);
    } else if (symbol == DO_WHILE_LOOP) {
        return DoWhileLoopScopeChange(node, childIndex, style);
    } else if (IsCompoundStatementLike(symbol)) {
        // This handles things that are enclosed in { and }
        // There are multiple grammar symbols that are handled the same way
        // but we will just refer to them all as compound statements.
        return CompoundStatementScopeChange(node, childIndex, style);
    }

    return ScopeChange::None;
}

}

namespace tree_sitter_format {

void IndentationTraverser::reset(const TraverserContext& context) {
    scope = 0;
    previousPosition = Position::StartOf(context.document.root());
}

void IndentationTraverser::visitLeaf(TSNode node, TraverserContext& context) {
    if (!context.style.indentation.reindent) {
        return;
    }

    Position position = Position::StartOf(node);

    uint32_t previousRow = previousPosition.location.row;
    uint32_t currentRow = position.location.row;

    // We only want to modify things if this is the first node on a line
    if (previousRow != currentRow) {
        Range preceedingWhitespace = context.document.toPreviousNewLine(position);

        // Delete the previous white space
        context.edits.push_back(DeleteEdit{.range = preceedingWhitespace});

        for(uint32_t i = 0; i < scope; i++) {
            context.edits.push_back(InsertEdit{.position = preceedingWhitespace.start, .bytes = context.style.indentationString()});
        }
    }

    previousPosition = Position::EndOf(node);
}

void IndentationTraverser::preVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context) {
    ScopeChange change = ScopeChangeForChild(node, childIndex, context.style);
    if (change == ScopeChange::IncreaseBefore || change == ScopeChange::Both) {
        scope++;
    }
}

void IndentationTraverser::postVisitChild(TSNode node, uint32_t childIndex, TraverserContext& context) {
    ScopeChange change = ScopeChangeForChild(node, childIndex, context.style);
    if (change == ScopeChange::DecreaseAfter || change == ScopeChange::Both) {
        scope--;
    }
}

}
