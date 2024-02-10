
#include <queue>
#include <set>
#include <unordered_map>

#include "../geometry/halfedge.h"
#include "debug.h"

/* Note on local operation return types:

    The local operations all return a std::optional<T> type. This is used so that your
    implementation can signify that it does not want to perform the operation for
    whatever reason (e.g. you don't want to allow the user to erase the last vertex).

    An optional can have two values: std::nullopt, or a value of the type it is
    parameterized on. In this way, it's similar to a pointer, but has two advantages:
    the value it holds need not be allocated elsewhere, and it provides an API that
    forces the user to check if it is null before using the value.

    In your implementaiton, if you have successfully performed the operation, you can
    simply return the required reference:

            ... collapse the edge ...
            return collapsed_vertex_ref;

    And if you wish to deny the operation, you can return the null optional:

            return std::nullopt;

    Note that the stubs below all reject their duties by returning the null optional.
*/

/*
    This method should replace the given vertex and all its neighboring
    edges and faces with a single face, returning the new face.
 */
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::erase_vertex(Halfedge_Mesh::VertexRef v) {

    (void)v;
    return std::nullopt;
}

/*
    This method should erase the given edge and return an iterator to the
    merged face.
 */
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::erase_edge(Halfedge_Mesh::EdgeRef e) {

    (void)e;
    return std::nullopt;
}

/*
    This method should collapse the given edge and return an iterator to
    the new vertex created by the collapse.
*/
std::optional<Halfedge_Mesh::VertexRef> Halfedge_Mesh::collapse_edge(Halfedge_Mesh::EdgeRef e) {
    // bail if last edge
    if(edges.size() < 2) {
        printf("Not enough edges\n");
        return std::nullopt;
    }
    // gather edge members
    auto he1 = e->halfedge();
    auto he2 = he1->twin();
    auto v1 = he1->vertex();
    auto v2 = he2->vertex();
    // create new vertex
    auto newVert = new_vertex();
    // set new vertex location to edge center
    newVert->pos = e->center();
    newVert->halfedge() = he1->next();
    // [1] iterate around vertices
    auto he = he1;
    // walk face to find half edge behind the collapsing edge
    //he = he->next()->next()->twin();
    while(he->next() != he1) {
        // print edge id
		he = he->next();
	}
    auto hnn1 = he; // store for later
    // walk round vertex to set new vertex
    he = he->twin();
    while(he != he1) {
        // set half edges to new vertex
        he->vertex() = newVert;
        auto end = he;
		//he = he->next()->next()->twin();
        while(he->next() != end) {
        	he = he->next();
        }
        he = he->twin();
    }
    // repeat [1]
    he = he2;
    while(he->next() != he2) {
        he = he->next();
    }
    auto hnn2 = he; // store for later
    he = he->twin();
    while (he != he2) {
        he->vertex() = newVert;
        auto end = he;
        while(he->next() != end) {
            he = he->next();
        }
        he = he->twin();
    }
    // snap twins together 
    // get twin half edges of next half edges
    if(he1->face()->degree() == 3) {
        auto hn1 = he1->next();
        auto hn1_edge = hn1->edge();
        // auto hnn1 = hn1->next();
        auto hnn1_edge = hnn1->edge();
        auto hn1_twin = hn1->twin();
        auto hnn1_twin = hnn1->twin();
        hn1_twin->twin() = hnn1_twin;
        hnn1_twin->twin() = hn1_twin;
        hnn1_twin->edge() = hn1_edge;
        hn1_edge->halfedge() = hn1_twin;
		erase(he1->face());
        erase(hnn1_edge);
        erase(hn1);
        erase(hnn1);
        hn1_twin->vertex()->halfedge() = hn1_twin;
        newVert->halfedge() = hnn1_twin;
    } else {
        hnn1->next() = he1->next();
		hnn1->face()->halfedge() = hnn1;
    }

    // get twin half edges of next half edges
    if (he2->face()->degree() == 3) {
		auto hn2 = he2->next();
		auto hn2_edge = hn2->edge();
		// auto hnn2 = hn2->next();
		auto hnn2_edge = hnn2->edge();
		auto hn2_twin = hn2->twin();
		auto hnn2_twin = hnn2->twin();
		hn2_twin->twin() = hnn2_twin;
		hnn2_twin->twin() = hn2_twin;
		hnn2_twin->edge() = hn2_edge;
		hn2_edge->halfedge() = hn2_twin;
        erase(hnn2_edge);
        erase(hn2);
        erase(hnn2);
        erase(he2->face());
        hn2_twin->vertex()->halfedge() = hn2_twin;
    } else {
        hnn2->next() = he2->next();
        hnn2->face()->halfedge() = hnn2;
    }
	// clean up old elements
    {
        erase(e);
        erase(he1);
        erase(he2);
        erase(v1);
        erase(v2);
    }
    return std::optional<Halfedge_Mesh::VertexRef>(newVert);
}

/*
    This method should collapse the given face and return an iterator to
    the new vertex created by the collapse.
*/
std::optional<Halfedge_Mesh::VertexRef> Halfedge_Mesh::collapse_face(Halfedge_Mesh::FaceRef f) {

    (void)f;
    return std::nullopt;
}

/*
    This method should flip the given edge and return an iterator to the
    flipped edge.
*/
std::optional<Halfedge_Mesh::EdgeRef> Halfedge_Mesh::flip_edge(Halfedge_Mesh::EdgeRef e) {
    // bail if is boundary edge
    if(e->on_boundary()) return std::nullopt;
    // get the origin verts
    // get the destination verts
    // get subject half edges
    // get the next half edges
    // get next next half edges
    auto he1 = e->halfedge();
    auto he2 = he1->twin();
    auto hn1 = he1->next();
    auto hn2 = he2->next();
    auto origin1 = hn1->vertex();
    auto origin2 = hn2->vertex();
    auto hnn1 = hn1->next();
    auto hnn2 = hn2->next();
    auto dest1 = hnn1->vertex();
    auto dest2 = hnn2->vertex();
    // search for half edge to origin1 => founde
    auto backhn1 = hnn1;
    while(backhn1->next() != he1) {
        backhn1 = backhn1->next();
    }
    // search for half edge to origin2
    auto backhn2 = hnn2;
    while(backhn2->next() != he2) {
        backhn2 = backhn2->next();
    }
    
    // begin CCW rotation

    // set current half edge to destination vertex
    // set twin to destination vertex
    // set current half edge next to destination next (next's next)
    // set current half edge twin next to destination next (next's next)
    // set next next to twin
    // twin next to destination 2 next (twin next's next)
    // set twin next next to current half edge
    he1->vertex() = dest2;
    he2->vertex() = dest1;
    he1->next() = hnn1;
    he2->next() = hnn2;
    hn1->next() = he2;
    hn2->next() = he1;

    // set to founde twin to twin next
    backhn1->next() = hn2;
    // set to current half edge next
    backhn2->next() = hn1;
    // set face to know value
    // set twin face to know value
    he1->face()->halfedge() = he1;
    he2->face()->halfedge() = he2;

    // ensure half edge points to face
    hn1->face() = he2->face();
    hn2->face() = he1->face();
    return std::optional<Halfedge_Mesh::EdgeRef>(e);
}

/*
    This method should split the given edge and return an iterator to the
    newly inserted vertex. The halfedge of this vertex should point along
    the edge that was split, rather than the new edges.
*/
std::optional<Halfedge_Mesh::VertexRef> Halfedge_Mesh::split_edge(Halfedge_Mesh::EdgeRef e) {
    // get the original elements
    auto he1 = e->halfedge();
    auto he2 = he1->twin();
    if(he1->is_boundary() || he2->is_boundary()) {
        printf("Boundary edges found\n");
        return std::nullopt;
	}

    auto hn1 = he1->next();
    auto hn2 = he2->next();
    auto hnn1 = hn1->next();
    auto hnn2 = hn2->next();
    auto A = he2->vertex();
    auto B = he1->vertex();
    auto C = hnn1->vertex();
    auto D = hnn2->vertex();
    auto oldFace = he1->face();
    auto oldFace2 = he2->face();

    // setup new variables
    Halfedge_Mesh::VertexRef midVert = new_vertex();
    auto edgeA = new_edge();
    auto edgeB = new_edge();
    auto edgeC = new_edge();
    auto edgeD = new_edge();
    auto nhn1 = new_halfedge();
    auto nhn2 = new_halfedge();
    auto nhn3 = new_halfedge();
    auto nhn4 = new_halfedge();
    auto nhe2 = new_halfedge();
    auto nhe4 = new_halfedge();
    auto nface1 = new_face();
    auto nface2 = new_face();
    auto nface3 = new_face();
    auto nface4 = new_face();

    // assign new values
    midVert->pos = e->center();
    midVert->halfedge() = nhn1;

    edgeA->halfedge() = he2;
    edgeB->halfedge() = he1;
    edgeC->halfedge() = nhn2;
    edgeD->halfedge() = nhn1;
    // init faces
    nface1->halfedge() = he2;
    nface2->halfedge() = nhe2;
    nface3->halfedge() = he1;
    nface4->halfedge() = nhe4;

    // init half edges
    // face 1
    {
		he2->next() = nhn1;
		he2->edge() = edgeA;
		he2->twin() = nhe2;
		he2->vertex() = A;
		he2->face() = nface1;
		nhn1->next() = hnn2;
		nhn1->edge() = edgeD;
		nhn1->twin() = nhn4;
		nhn1->vertex() = midVert;
		nhn1->face() = nface1;
		hnn2->next() = he2;
		hnn2->edge() = hnn2->edge();
		hnn2->twin() = hnn2->twin();
		hnn2->vertex() = D;
		hnn2->face() = nface1;
	}
    // face 2
    {
     	nhe2->next() = hn1;
     	nhe2->edge() = edgeA;
     	nhe2->twin() = he2;
     	nhe2->vertex() = midVert;
     	nhe2->face() = nface2;
     	nhn2->next() = nhe2;
     	nhn2->edge() = edgeC;
     	nhn2->twin() = nhn3;
     	nhn2->vertex() = C;
     	nhn2->face() = nface2;
        hn1->next() = nhn2;
        hn1->edge() = hn1->edge();
        hn1->twin() = hn1->twin();
        hn1->vertex() = A;
        hn1->face() = nface2;
    }
    // face 3
    {
        he1->next() = nhn3;
        he1->edge() = edgeB;
        he1->twin() = nhe4;
        he1->vertex() = B;
        he1->face() = nface3;
        nhn3->next() = hnn1;
        nhn3->edge() = edgeC;
        nhn3->twin() = nhn2;
        nhn3->vertex() = midVert;
        nhn3->face() = nface3;
        hnn1->next() = he1;
        hnn1->edge() = hnn1->edge();
        hnn1->twin() = hnn1->twin();
        hnn1->vertex() = C;
        hnn1->face() = nface3;
    }
    // face 4
    {
     	nhe4->next() = hn2;
     	nhe4->edge() = edgeB;
     	nhe4->twin() = he1;
     	nhe4->vertex() = midVert;
     	nhe4->face() = nface4;
     	nhn4->next() = nhe4;
     	nhn4->edge() = edgeD;
     	nhn4->twin() = nhn1;
     	nhn4->vertex() = D;
     	nhn4->face() = nface4;
     	hn2->next() = nhn4;
     	hn2->edge() = hn2->edge();
     	hn2->twin() = hn2->twin();
     	hn2->vertex() = B;
     	hn2->face() = nface4;
    }

    // delete unused elements
    erase(e);
    erase(oldFace);
    erase(oldFace2);


    return std::optional<Halfedge_Mesh::VertexRef>(midVert);
}

/* Note on the beveling process:

    Each of the bevel_vertex, bevel_edge, and bevel_face functions do not represent
    a full bevel operation. Instead, they should update the _connectivity_ of
    the mesh, _not_ the positions of newly created vertices. In fact, you should set
    the positions of new vertices to be exactly the same as wherever they "started from."

    When you click on a mesh element while in bevel mode, one of those three functions
    is called. But, because you may then adjust the distance/offset of the newly
    beveled face, we need another method of updating the positions of the new vertices.

    This is where bevel_vertex_positions, bevel_edge_positions, and
    bevel_face_positions come in: these functions are called repeatedly as you
    move your mouse, the position of which determins the normal and tangent offset
    parameters. These functions are also passed an array of the original vertex
    positions: for  bevel_vertex, it has one element, the original vertex position,
    for bevel_edge,  two for the two vertices, and for bevel_face, it has the original
    position of each vertex in halfedge order. You should use these positions, as well
    as the normal and tangent offset fields to assign positions to the new vertices.

    Finally, note that the normal and tangent offsets are not relative values - you
    should compute a particular new position from them, not a delta to apply.
*/

/*
    This method should replace the vertex v with a face, corresponding to
    a bevel operation. It should return the new face.  NOTE: This method is
    responsible for updating the *connectivity* of the mesh only---it does not
    need to update the vertex positions.  These positions will be updated in
    Halfedge_Mesh::bevel_vertex_positions (which you also have to
    implement!)
*/
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::bevel_vertex(Halfedge_Mesh::VertexRef v) {

    // Reminder: You should set the positions of new vertices (v->pos) to be exactly
    // the same as wherever they "started from."

    (void)v;
    return std::nullopt;
}

/*
    This method should replace the edge e with a face, corresponding to a
    bevel operation. It should return the new face. NOTE: This method is
    responsible for updating the *connectivity* of the mesh only---it does not
    need to update the vertex positions.  These positions will be updated in
    Halfedge_Mesh::bevel_edge_positions (which you also have to
    implement!)
*/
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::bevel_edge(Halfedge_Mesh::EdgeRef e) {

    // Reminder: You should set the positions of new vertices (v->pos) to be exactly
    // the same as wherever they "started from."

    (void)e;
    return std::nullopt;
}

/*
    This method should replace the face f with an additional, inset face
    (and ring of faces around it), corresponding to a bevel operation. It
    should return the new face.  NOTE: This method is responsible for updating
    the *connectivity* of the mesh only---it does not need to update the vertex
    positions. These positions will be updated in
    Halfedge_Mesh::bevel_face_positions (which you also have to
    implement!)
*/
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::bevel_face(Halfedge_Mesh::FaceRef f) {

    // Reminder: You should set the positions of new vertices (v->pos) to be exactly
    // the same as wherever they "started from."

    (void)f;
    return std::nullopt;
}

/*
    Compute new vertex positions for the vertices of the beveled vertex.

    These vertices can be accessed via new_halfedges[i]->vertex()->pos for
    i = 1, ..., new_halfedges.size()-1.

    The basic strategy here is to loop over the list of outgoing halfedges,
    and use the original vertex position and its associated outgoing edge
    to compute a new vertex position along the outgoing edge.
*/
void Halfedge_Mesh::bevel_vertex_positions(const std::vector<Vec3>& start_positions,
                                           Halfedge_Mesh::FaceRef face, float tangent_offset) {

    std::vector<HalfedgeRef> new_halfedges;
    auto h = face->halfedge();
    do {
        new_halfedges.push_back(h);
        h = h->next();
    } while(h != face->halfedge());

    (void)new_halfedges;
    (void)start_positions;
    (void)face;
    (void)tangent_offset;
}

/*
    Compute new vertex positions for the vertices of the beveled edge.

    These vertices can be accessed via new_halfedges[i]->vertex()->pos for
    i = 1, ..., new_halfedges.size()-1.

    The basic strategy here is to loop over the list of outgoing halfedges,
    and use the preceding and next vertex position from the original mesh
    (in the orig array) to compute an offset vertex position.

    Note that there is a 1-to-1 correspondence between halfedges in
    newHalfedges and vertex positions
    in orig.  So, you can write loops of the form

    for(size_t i = 0; i < new_halfedges.size(); i++)
    {
            Vector3D pi = start_positions[i]; // get the original vertex
            position corresponding to vertex i
    }
*/
void Halfedge_Mesh::bevel_edge_positions(const std::vector<Vec3>& start_positions,
                                         Halfedge_Mesh::FaceRef face, float tangent_offset) {

    std::vector<HalfedgeRef> new_halfedges;
    auto h = face->halfedge();
    do {
        new_halfedges.push_back(h);
        h = h->next();
    } while(h != face->halfedge());

    (void)new_halfedges;
    (void)start_positions;
    (void)face;
    (void)tangent_offset;
}

/*
    Compute new vertex positions for the vertices of the beveled face.

    These vertices can be accessed via new_halfedges[i]->vertex()->pos for
    i = 1, ..., new_halfedges.size()-1.

    The basic strategy here is to loop over the list of outgoing halfedges,
    and use the preceding and next vertex position from the original mesh
    (in the start_positions array) to compute an offset vertex
    position.

    Note that there is a 1-to-1 correspondence between halfedges in
    new_halfedges and vertex positions
    in orig. So, you can write loops of the form

    for(size_t i = 0; i < new_halfedges.size(); i++)
    {
            Vec3 pi = start_positions[i]; // get the original vertex
            position corresponding to vertex i
    }
*/
void Halfedge_Mesh::bevel_face_positions(const std::vector<Vec3>& start_positions,
                                         Halfedge_Mesh::FaceRef face, float tangent_offset,
                                         float normal_offset) {

    if(flip_orientation) normal_offset = -normal_offset;
    std::vector<HalfedgeRef> new_halfedges;
    auto h = face->halfedge();
    do {
        new_halfedges.push_back(h);
        h = h->next();
    } while(h != face->halfedge());

    (void)new_halfedges;
    (void)start_positions;
    (void)face;
    (void)tangent_offset;
    (void)normal_offset;
}

/*
    Splits all non-triangular faces into triangles.
*/
void Halfedge_Mesh::triangulate() {
    // iterate over faces
    std::vector<Halfedge_Mesh::FaceRef> notTris;
    for (auto f = faces_begin(); f != faces_end(); f++) {
        if (f->degree() != 3) {
			notTris.push_back(f);
		}
	}
    for(auto face : notTris) {
        auto he = face->halfedge();
            // while curr degree not 3
        while(he->face()->degree() != 3) {
            auto temp_face = he->face();
            if(temp_face->degree() == 3)
                printf("here\n");
            // create 2 face and edge
            // if face is not a triangle split
            // find home vertex
            auto hn = he->next();
            auto hnn = hn->next();
            // create new edge
            auto newEdge = new_edge();
            // create its half edges
            auto newHe = new_halfedge();
            newHe->vertex() = hnn->vertex();
            newHe->next() = he;
            auto newHeTwin = new_halfedge();
            newHeTwin->vertex() = he->vertex();
            newHeTwin->next() = hnn;
            newHe->twin() = newHeTwin;
            newHeTwin->twin() = newHe;
            newHe->edge() = newEdge;
            newHeTwin->edge() = newEdge;
            newEdge->halfedge() = newHe;

            auto new_face1 = new_face();
            auto new_face2 = new_face();

            // reassign half edges
            hn->next() = newHe;
            auto temp2 = hnn;
            while (temp2->next() != he) {
				temp2 = temp2->next();
			}
            temp2->next() = newHeTwin;

            auto temp = he;
            new_face1->halfedge() = he;
            do {
			    temp->face() = new_face1;
			    temp = temp->next();
		    } while(temp != he);
            temp = newHeTwin;
            new_face2->halfedge() = newHeTwin;
            do {
                temp->face() = new_face2;
                temp = temp->next();
            } while(temp != newHeTwin);
            if(temp_face->id() == 33) {
				printf("here\n");
            }
            erase(temp_face);
            he = newHeTwin;
		}

    }
}

/* Note on the quad subdivision process:

        Unlike the local mesh operations (like bevel or edge flip), we will perform
        subdivision by splitting *all* faces into quads "simultaneously."  Rather
        than operating directly on the halfedge data structure (which as you've
        seen is quite difficult to maintain!) we are going to do something a bit nicer:
           1. Create a raw list of vertex positions and faces (rather than a full-
              blown halfedge mesh).
           2. Build a new halfedge mesh from these lists, replacing the old one.
        Sometimes rebuilding a data structure from scratch is simpler (and even
        more efficient) than incrementally modifying the existing one.  These steps are
        detailed below.

  Step I: Compute the vertex positions for the subdivided mesh.
        Here we're going to do something a little bit strange: since we will
        have one vertex in the subdivided mesh for each vertex, edge, and face in
        the original mesh, we can nicely store the new vertex *positions* as
        attributes on vertices, edges, and faces of the original mesh. These positions
        can then be conveniently copied into the new, subdivided mesh.
        This is what you will implement in linear_subdivide_positions() and
        catmullclark_subdivide_positions().

  Steps II-IV are provided (see Halfedge_Mesh::subdivide()), but are still detailed
  here:

  Step II: Assign a unique index (starting at 0) to each vertex, edge, and
        face in the original mesh. These indices will be the indices of the
        vertices in the new (subdivided mesh).  They do not have to be assigned
        in any particular order, so long as no index is shared by more than one
        mesh element, and the total number of indices is equal to V+E+F, i.e.,
        the total number of vertices plus edges plus faces in the original mesh.
        Basically we just need a one-to-one mapping between original mesh elements
        and subdivided mesh vertices.

  Step III: Build a list of quads in the new (subdivided) mesh, as tuples of
        the element indices defined above. In other words, each new quad should be
        of the form (i,j,k,l), where i,j,k and l are four of the indices stored on
        our original mesh elements.  Note that it is essential to get the orientation
        right here: (i,j,k,l) is not the same as (l,k,j,i).  Indices of new faces
        should circulate in the same direction as old faces (think about the right-hand
        rule).

  Step IV: Pass the list of vertices and quads to a routine that clears
        the internal data for this halfedge mesh, and builds new halfedge data from
        scratch, using the two lists.
*/

/*
    Compute new vertex positions for a mesh that splits each polygon
    into quads (by inserting a vertex at the face midpoint and each
    of the edge midpoints).  The new vertex positions will be stored
    in the members Vertex::new_pos, Edge::new_pos, and
    Face::new_pos.  The values of the positions are based on
    simple linear interpolation, e.g., the edge midpoints and face
    centroids.
*/
void Halfedge_Mesh::linear_subdivide_positions() {

    // For each vertex, assign Vertex::new_pos to
    // its original position, Vertex::pos.
    // For each edge, assign the midpoint of the two original
    // positions to Edge::new_pos.
    // For each face, assign the centroid (i.e., arithmetic mean)
    // of the original vertex positions to Face::new_pos. Note
    // that in general, NOT all faces will be triangles!

    for(auto v = vertices_begin(); v != vertices_end(); v++) {
        v->new_pos = v->pos;
    }
    for(auto e = edges_begin(); e != edges_end(); e++) {
        e->new_pos = e->center();
    }
    for(auto f = faces_begin(); f != faces_end(); f++) {
        f->new_pos = f->center();
    }
}
/*
    Compute new vertex positions for a mesh that splits each polygon
    into quads (by inserting a vertex at the face midpoint and each
    of the edge midpoints).  The new vertex positions will be stored
    in the members Vertex::new_pos, Edge::new_pos, and
    Face::new_pos.  The values of the positions are based on
    the Catmull-Clark rules for subdivision.

    Note: this will only be called on meshes without boundary
*/
void Halfedge_Mesh::catmullclark_subdivide_positions() {

    // The implementation for this routine should be
    // a lot like Halfedge_Mesh:linear_subdivide_positions:(),
    // except that the calculation of the positions themsevles is
    // slightly more involved, using the Catmull-Clark subdivision
    // rules. (These rules are outlined in the Developer Manual.)

    // Faces
    for (auto f = faces_begin(); f != faces_end(); f++) {
		f->new_pos = f->center();
	}
    // Edges
    for (auto e = edges_begin(); e != edges_end(); e++) {
        auto f1 = e->halfedge()->face();
        auto f2 = e->halfedge()->twin()->face();
        auto v1 = e->halfedge()->vertex();
        auto v2 = e->halfedge()->twin()->vertex();
        e->new_pos = (f1->new_pos + f2->new_pos + v1->pos + v2->pos) / 4.0f;
	}
    // Vertices
    for (auto v = vertices_begin(); v != vertices_end(); v++) {
		auto he = v->halfedge();
		float N = v->degree();
        float n = 1.0f / N;
		Vec3 faceSum = Vec3(0, 0, 0);
		Vec3 edgeSum = Vec3(0, 0, 0);
        do {
			faceSum += he->face()->new_pos;
			edgeSum += he->edge()->new_pos;
			he = he->twin()->next();
		} while (he != v->halfedge());
        faceSum *= n;
        edgeSum *= n;
		v->new_pos = (faceSum + 2 * edgeSum + (N - 3) * v->pos) * n;
    }
}

/*
        This routine should increase the number of triangles in the mesh
        using Loop subdivision. Note: this is will only be called on triangle meshes.
*/
void Halfedge_Mesh::loop_subdivide() {

    // Compute new positions for all the vertices in the input mesh, using
    // the Loop subdivision rule, and store them in Vertex::new_pos.
    // -> At this point, we also want to mark each vertex as being a vertex of the
    //    original mesh. Use Vertex::is_new for this.
    // -> Next, compute the updated vertex positions associated with edges, and
    //    store it in Edge::new_pos.
    // -> Next, we're going to split every edge in the mesh, in any order.  For
    //    future reference, we're also going to store some information about which
    //    subdivided edges come from splitting an edge in the original mesh, and
    //    which edges are new, by setting the flat Edge::is_new. Note that in this
    //    loop, we only want to iterate over edges of the original mesh.
    //    Otherwise, we'll end up splitting edges that we just split (and the
    //    loop will never end!)
    // -> Now flip any new edge that connects an old and new vertex.
    // -> Finally, copy the new vertex positions into final Vertex::pos.

    // Each vertex and edge of the original surface can be associated with a
    // vertex in the new (subdivided) surface.
    // Therefore, our strategy for computing the subdivided vertex locations is to
    // *first* compute the new positions
    // using the connectivity of the original (coarse) mesh; navigating this mesh
    // will be much easier than navigating
    // the new subdivided (fine) mesh, which has more elements to traverse.  We
    // will then assign vertex positions in
    // the new mesh based on the values we computed for the original mesh.

    // Compute updated positions for all the vertices in the original mesh, using
    // the Loop subdivision rule.

    // Next, compute the updated vertex positions associated with edges.

    // Next, we're going to split every edge in the mesh, in any order. For
    // future reference, we're also going to store some information about which
    // subdivided edges come from splitting an edge in the original mesh, and
    // which edges are new.
    // In this loop, we only want to iterate over edges of the original
    // mesh---otherwise, we'll end up splitting edges that we just split (and
    // the loop will never end!)

    // Finally, flip any new edge that connects an old and new vertex.

    // Copy the updated vertex positions to the subdivided mesh.
}

/*
    Isotropic remeshing. Note that this function returns success in a similar
    manner to the local operations, except with only a boolean value.
    (e.g. you may want to return false if this is not a triangle mesh)
*/
bool Halfedge_Mesh::isotropic_remesh() {

    // Compute the mean edge length.
    // Repeat the four main steps for 5 or 6 iterations
    // -> Split edges much longer than the target length (being careful about
    //    how the loop is written!)
    // -> Collapse edges much shorter than the target length.  Here we need to
    //    be EXTRA careful about advancing the loop, because many edges may have
    //    been destroyed by a collapse (which ones?)
    // -> Now flip each edge if it improves vertex degree
    // -> Finally, apply some tangential smoothing to the vertex positions

    // Note: if you erase elements in a local operation, they will not be actually deleted
    // until do_erase or validate are called. This is to facilitate checking
    // for dangling references to elements that will be erased.
    // The rest of the codebase will automatically call validate() after each op,
    // but here simply calling collapse_edge() will not erase the elements.
    // You should use collapse_edge_erase() instead for the desired behavior.

    return false;
}

/* Helper type for quadric simplification */
struct Edge_Record {
    Edge_Record() {
    }
    Edge_Record(std::unordered_map<Halfedge_Mesh::VertexRef, Mat4>& vertex_quadrics,
                Halfedge_Mesh::EdgeRef e)
        : edge(e) {

        // Compute the combined quadric from the edge endpoints.
        // -> Build the 3x3 linear system whose solution minimizes the quadric error
        //    associated with these two endpoints.
        // -> Use this system to solve for the optimal position, and store it in
        //    Edge_Record::optimal.
        // -> Also store the cost associated with collapsing this edge in
        //    Edge_Record::cost.
        auto blah = e->halfedge()->vertex();
        Mat4 quad1 = vertex_quadrics[blah];
        Mat4 quad2 = vertex_quadrics[e->halfedge()->twin()->vertex()];
        Mat4 Q = quad1 + quad2;
        Vec3 b = -Vec3(Q[3][0], Q[3][1], Q[3][2]);
        optimal = Q.inverse() * b;
        // cost is optimal transpose * Q * optimal
        cost = dot(optimal, Q * optimal);
    }
    Halfedge_Mesh::EdgeRef edge;
    Vec3 optimal;
    float cost;
};

/* Comparison operator for Edge_Records so std::set will properly order them */
bool operator<(const Edge_Record& r1, const Edge_Record& r2) {
    if(r1.cost != r2.cost) {
        return r1.cost < r2.cost;
    }
    Halfedge_Mesh::EdgeRef e1 = r1.edge;
    Halfedge_Mesh::EdgeRef e2 = r2.edge;
    return &*e1 < &*e2;
}

/** Helper type for quadric simplification
 *
 * A PQueue is a minimum-priority queue that
 * allows elements to be both inserted and removed from the
 * queue.  Together, one can easily change the priority of
 * an item by removing it, and re-inserting the same item
 * but with a different priority.  A priority queue, for
 * those who don't remember or haven't seen it before, is a
 * data structure that always keeps track of the item with
 * the smallest priority or "score," even as new elements
 * are inserted and removed.  Priority queues are often an
 * essential component of greedy algorithms, where one wants
 * to iteratively operate on the current "best" element.
 *
 * PQueue is templated on the type T of the object
 * being queued.  For this reason, T must define a comparison
 * operator of the form
 *
 *    bool operator<( const T& t1, const T& t2 )
 *
 * which returns true if and only if t1 is considered to have a
 * lower priority than t2.
 *
 * Basic use of a PQueue might look
 * something like this:
 *
 *    // initialize an empty queue
 *    PQueue<myItemType> queue;
 *
 *    // add some items (which we assume have been created
 *    // elsewhere, each of which has its priority stored as
 *    // some kind of internal member variable)
 *    queue.insert( item1 );
 *    queue.insert( item2 );
 *    queue.insert( item3 );
 *
 *    // get the highest priority item currently in the queue
 *    myItemType highestPriorityItem = queue.top();
 *
 *    // remove the highest priority item, automatically
 *    // promoting the next-highest priority item to the top
 *    queue.pop();
 *
 *    myItemType nextHighestPriorityItem = queue.top();
 *
 *    // Etc.
 *
 *    // We can also remove an item, making sure it is no
 *    // longer in the queue (note that this item may already
 *    // have been removed, if it was the 1st or 2nd-highest
 *    // priority item!)
 *    queue.remove( item2 );
 *
 */
template<class T> struct PQueue {
    void insert(const T& item) {
        queue.insert(item);
    }
    void remove(const T& item) {
        if(queue.find(item) != queue.end()) {
            queue.erase(item);
        }
    }
    const T& top(void) const {
        return *(queue.begin());
    }
    void pop(void) {
        queue.erase(queue.begin());
    }
    size_t size() {
        return queue.size();
    }

    std::set<T> queue;
};

/*
    Mesh simplification. Note that this function returns success in a similar
    manner to the local operations, except with only a boolean value.
    (e.g. you may want to return false if you can't simplify the mesh any
    further without destroying it.)
*/
bool Halfedge_Mesh::simplify() {
    if (edges.size() < 3) {
		return false;
	}
    std::unordered_map<VertexRef, Mat4> vertex_quadrics;
    std::unordered_map<FaceRef, Mat4> face_quadrics;
    std::unordered_map<EdgeRef, Edge_Record> edge_records;
    PQueue<Edge_Record> edge_queue;
    // Compute initial quadrics for each face by simply writing the plane equation
    // for the face in homogeneous coordinates. These quadrics should be stored
    // in face_quadrics
    // -> Compute an initial quadric for each vertex as the sum of the quadrics
    //    associated with the incident faces, storing it in vertex_quadrics
    // -> Build a priority queue of edges according to their quadric error cost,
    //    i.e., by building an Edge_Record for each edge and sticking it in the
    //    queue. You may want to use the above PQueue<Edge_Record> for this.
    // -> Until we reach the target edge budget, collapse the best edge. Remember
    //    to remove from the queue any edge that touches the collapsing edge
    //    BEFORE it gets collapsed, and add back into the queue any edge touching
    //    the collapsed vertex AFTER it's been collapsed. Also remember to assign
    //    a quadric to the collapsed vertex, and to pop the collapsed edge off the
    //    top of the queue.
    for (auto f = faces_begin(); f != faces_end(); f++)
    {
		Vec3 normal = f->normal();
		Vec3 point = f->halfedge()->vertex()->pos;
		float d = -dot(normal, point);
		Mat4 quadric = outer(Vec4(point, 1.0f), Vec4(normal, d));
		face_quadrics[f] = quadric;
	}
    for (auto v = vertices_begin(); v != vertices_end(); v++)
    {
        Mat4 quadric = Mat4();
        auto he = v->halfedge();
        do {
            quadric += face_quadrics[he->face()];
			he = he->twin()->next();
		} while (he != v->halfedge());
        vertex_quadrics[v] = quadric;
    }
    for (auto e = edges_begin(); e != edges_end(); e++)
    {
		edge_records[e] = Edge_Record(vertex_quadrics, e);
        edge_queue.insert(Edge_Record(vertex_quadrics, e));
	}
    int target = edges.size() * 0.75;
    while(edges.size() > target)
	{
        Edge_Record best = edge_queue.top();
        edge_queue.pop();
        auto e = best.edge;
        auto v1 = e->halfedge()->vertex();
        auto v2 = e->halfedge()->twin()->vertex();
        Mat4 Q = vertex_quadrics[v1] + vertex_quadrics[v2];
        vertex_quadrics.erase(v1);
        vertex_quadrics.erase(v2);
        auto he = v1->halfedge();
        do {
            edge_queue.remove(edge_records[he->edge()]);
            edge_records.erase(he->edge());
            he = he->twin()->next();
		} while (he != v1->halfedge());
        he = v2->halfedge();
        do {
            auto temp = he->edge();
            auto er = edge_records[temp];
            edge_queue.remove(er);
            edge_records.erase(temp);
            he = he->twin()->next();
        } while (he != v2->halfedge());

        auto newVert = collapse_edge_erase(e);
        
        vertex_quadrics[newVert.value()] = Q;
        he = newVert.value()->halfedge();
        do {
            auto er = Edge_Record(vertex_quadrics, he->edge());
			edge_records[he->edge()] = er;
			edge_queue.insert(edge_records[he->edge()]);
			he = he->twin()->next();
		} while (he != newVert.value()->halfedge());
    }
    // Note: if you erase elements in a local operation, they will not be actually deleted
    // until do_erase or validate are called. This is to facilitate checking
    // for dangling references to elements that will be erased.
    // The rest of the codebase will automatically call validate() after each op,
    // but here simply calling collapse_edge() will not erase the elements.
    // You should use collapse_edge_erase() instead for the desired behavior.

    return true;
}
