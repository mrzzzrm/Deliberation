#pragma once

namespace deliberation
{

enum PrimitiveType
{
    Primitive_Triangles,
    Primitive_Triangle_Strip,
    Primitive_Lines,
    Primitive_Line_Strip,
};

enum QueryType
{
    Query_TimeElapsed,
    Query_SamplesPassed,
    Query_AnySamplesPassed,
    Query_PrimitivesGenerated,
    Query_TransformFeedbackPrimitives
};

}

