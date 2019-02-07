    classdef GraphModel < handle
    %GraphModel Represents a graph physcal model, which can contain several
    % strings and connections, as well as boundary conditions for specific
    % nodes. Functions are provided to compute the second and fourth order
    % spatial difference operators, which can be used to implement physical
    % models of strings, bars, plates and so on.
    %
    % Basic usage:
    %
    %   % Create graph
    %   m = GraphModel();
    %   
    %   % Add strings
    %   s1 = m.addString(10);
    %   s2 = m.addString(10);
    %
    %   % Connect strings
    %   m.connectStrings(s1, s2);
    %
    %   % Add boundary conditions
    %   m.addDrichlet(s1, 1);
    %   m.addNeumann(s2, 10);
    %
    %  [Implement your model using m.getDxx(s, p) or m.getDxxxx(s, p)]
    
    properties
        % Cell array holding the deformation of the strings.
        strings
        
        % Cell array holding the previous deformation of the strings.
        strings1
        
        % Cell array holding the temporary deformation of the strings.
        stringsTemp
        
        % Array describing the connections between strings.
        connections
        
        % Array describing which nodes have Dirichlet boundary conditions.
        dirichlet
        
        % Array describing which nodes have Neumann boundary conditions.
        neumann
    end
    
    methods
        function obj = GraphModel()
            %MODEL Construct a new empty model class.
            obj.strings = {};
            obj.connections = reshape([], [0, 2]);
            obj.dirichlet = reshape([], [0, 2]);
            obj.neumann = reshape([], [0, 2]);
        end
        
        function stringId = addString(this, L)
            % Add a new string to the model model
            %
            % stringId = addString(L) Creates a string of length L and
            % returns the ID of that string.
            this.strings{end+1} = zeros(1, L);
            this.stringsTemp = this.strings;
            this.strings1 = this.strings;
            stringId = length(this.strings);
        end
        
        function [] = addDirichlet(this, stringId, position)
            % Add a Dririchlet boundary condition to a node.
            %
            % addDirichlet(s, p) adds the Dirichlet boundary condition to
            % node p in string s.
            this.dirichlet(end+1, :) = [stringId, position];
        end
        
        function [] = addNeumann(this, stringId, position)
            % Add a Neumann boundary condition to a node.
            %
            % addDirichlet(s, p) adds the Neumann boundary condition to
            % node p in string s.
            this.neumann(end+1, :) = [stringId, position];
        end

        function [] = connectStrings(this, stringIdA, stringIdB)
            % Connect two strings within the graph.
            %
            % GraphModel.connect(s1, s2)
            % Connects string `s2` to the end of string `s1`.
            
            if stringIdA < 0 || stringIdA > length(this.strings)
                error("Invalid string A ID %i", stringIdA);
            end

            if stringIdB < 0 || stringIdB > length(this.strings)
                error("Invalid string B ID %i", stringIdB);
            end

            if ismember([stringIdA stringIdB], this.connections, 'rows')
                error("Strings already connected");
            end

            this.connections(end+1, :) = [stringIdA, stringIdB];
        end
        
        function [nodes] = getSorroundingNodes(this, stringId, position)
            % Get the nodes sorrounding the nodes of a given node.
            %
            % [nodes] = GraphModel.getSorroundingNodes(s, p)
            % Gets the sorrunding nodes of node `p` in string `s`. `nodes`
            % has the format `[stringId, position]`.
        
            l = size(this.strings{stringId}, 2);
            
            if position > 1 && position < l
               nodes = [stringId, position - 1; stringId position + 1]; 
            elseif position == 1
                nodes = [];
                % Get all string connected at the beginning of stringId
                connected = this.connections(this.connections(:, 2) == stringId, 1);

                for s = connected'
                    nodes = [nodes; s, size(this.strings{s}, 2)];
                end

                nodes = [nodes; stringId, position + 1];
            else
                nodes = [stringId, position - 1];
                
                % Get all string connected at the end of the string
                connected = this.connections(this.connections(:, 1) == stringId, 2);
             
                for s = connected'
                    nodes = [nodes; s, 1];
                end
            end
        end
        
        function [d] = getDxx(this, stringId, position)
            % Get the discrete second order derivative of a node.
            %
            % [d] = GraphModel.getDxx(stirngId, position)
            % Gets the discrete second order derivative of the node at
            % `position` in the string with string ID `stringId`.
        
            sorrounding = this.getSorroundingNodes(stringId, position);
            d = 0;
            
            for i = 1:size(sorrounding, 1)
                s = sorrounding(i, :);
                d = d + this.strings{s(1)}(s(2));
            end

            if ismember([stringId, position], this.dirichlet, 'rows')
                d = d - 2 * size(sorrounding, 1) * this.strings{stringId}(position);
            elseif ismember([stringId, position], this.neumann, 'rows')
                d = 2 * d - 2 * size(sorrounding, 1) * this.strings{stringId}(position);
            else
                d = d - size(sorrounding, 1) * this.strings{stringId}(position);
            end   
        end
        
        function [d] = getDxxxx(this, stringId, position)
            % Get the discrete fourth order derivative of a node.
            %
            % [d] = GraphModel.getDxx(stirngId, position)
            % Gets the discrete second order derivative of the node at
            % `position` in the string with string ID `stringId`.
        
            sorrounding = this.getSorroundingNodes(stringId, position);
            d = 0;
            
            for i = 1:size(sorrounding, 1)
               s = sorrounding(i, :);
               d = d + this.getDxx(s(1), s(2));
            end
            
            if ismember([stringId, position], this.dirichlet, 'rows')
                d = d - 2 * size(sorrounding, 1) * this.getDxx(stringId, position);
            elseif ismember([stringId, position], this.neumann, 'rows')
                d = 2 * d - 2 * size(sorrounding, 1) * this.getDxx(stringId, position);
            else
                d = d - size(sorrounding, 1) * this.getDxx(stringId, position);
            end            
        end
        
        function [s] = getNodeCount(this)
            % Get the total number of nodes.
            %
            % [s] = GraphModel.getNodeCount()
            % Gets the total number of nodes `s` in the graph
            s = 0;
            
            for string = this.strings
                s = s + length(string{1});
            end
        end
    end
end

