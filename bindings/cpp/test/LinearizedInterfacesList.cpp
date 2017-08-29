#include <joint.cpp/Component.hpp>


namespace joint {
namespace detail {
namespace test
{

	struct N11 { typedef TypeList<> BaseInterfaces; };
	struct N12 { typedef TypeList<> BaseInterfaces; };
	struct N21 { typedef TypeList<> BaseInterfaces; };
	struct N22 { typedef TypeList<> BaseInterfaces; };
	struct N23 { typedef TypeList<> BaseInterfaces; };
	struct N31 { typedef TypeList<> BaseInterfaces; };
	struct N1 { typedef TypeList<N11, N12> BaseInterfaces; };
	struct N2 { typedef TypeList<N21, N22, N23> BaseInterfaces; };
	struct N3 { typedef TypeList<N31> BaseInterfaces; };
	struct Root { typedef TypeList<N1, N2, N3> JointInterfaces; };


	template < typename >
	class LinearizedInterfacesListTest;

	template < >
	class LinearizedInterfacesListTest<TypeListNode<N1, TypeListNode<N12, TypeListNode<N2, TypeListNode<N22, TypeListNode<N23, TypeListNode<N3, TypeListEndNode> > > > > > >
	{ };


	LinearizedInterfacesListTest<
		typename LinearizedInterfacesList<Root::JointInterfaces>::ValueT
	> ERROR__LinearizedInterfacesListTest_failed;

}}}
